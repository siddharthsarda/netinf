#include "stdafx.h"
#include "cascinf.h"

void SaveEdges(const TStr& OutFNm, const TIntPrIntH& EdgesUsed) {
	// Save Used Edges
	  TFOut FOut(OutFNm);

	  // write nodes to file
	  for (TIntPrIntH::TIter EI = EdgesUsed.BegI(); EI < EdgesUsed.EndI(); EI++) {
	  	FOut.PutStr(TStr::Fmt("%d,%d,%d\r\n", EI.GetKey().Val1.Val, EI.GetKey().Val2.Val, EI.GetDat().Val)); // nodes
	  }

	  FOut.PutStr("\r\n");
}

int main(int argc, char* argv[]) {
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("\nGenerate different synthetic networks & cascades. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  TExeTm ExeTm;
  Try

  const int TNetwork = Env.GetIfArgPrefixInt("-t:", 0, "Type of network to generate\n\
  	  0:kronecker, 1:forest fire (default:0)\n");
  const TStr NetworkParams = Env.GetIfArgPrefixStr("-g:", TStr("0.9 0.5; 0.5 0.9"), "Parameters for the network (default:0.9 0.5; 0.5 0.9)\n");

  const int NNodes = Env.GetIfArgPrefixInt("-n:", 512, "Number of nodes (default:100)\n");
  const int NEdges = Env.GetIfArgPrefixInt("-e:", 1024, "Number of edges (default:100)\n");
  const int NCascades = Env.GetIfArgPrefixInt("-c:", -95, "If positive, number of cascades, if negative, percentage of edges used at least in one cascade (default:-95)\n");
  const int TModel = Env.GetIfArgPrefixInt("-m:", 0, "Transmission model\n0:exponential, 1:power law, 2:rayleigh (default:0)\n");
  const TStr RAlphas = Env.GetIfArgPrefixStr("-a:", TStr("0.01;10"), "Minimum and maximum value for alpha (default:0.01;10)\n");
  const TStr RBetas = Env.GetIfArgPrefixStr("-b:", TStr("0.5;0.5"), "Minimum and maximum value for beta (default:0.5;0.5)\n");
  const double Delta = Env.GetIfArgPrefixFlt("-d:", 1.0, "Delta for power-law (default:1)\n");
  const double TimeHorizon = Env.GetIfArgPrefixFlt("-h:", 100.0, "Time Horizon (default:100)\n");

  // we allow for mixture of exponentials
  const TStr FileName = Env.GetIfArgPrefixStr("-f:", TStr("example"), "Name of the network (default:example)\n");

  TNetInfBs NetInfBs;

  // Generate GroundTruth and Save
  NetInfBs.GenerateGroundTruth(TNetwork, NNodes, NEdges, NetworkParams);

  TStrV RAlphasV; RAlphas.SplitOnAllCh(';', RAlphasV);
  TStrV RBetasV; RBetas.SplitOnAllCh(';', RBetasV);
  NetInfBs.SetModels(RAlphasV[0].GetFlt(), RAlphasV[1].GetFlt(), RBetasV[0].GetFlt(), RBetasV[1].GetFlt());

  NetInfBs.SaveGroundTruth(TStr::Fmt("%s-network.txt", FileName.CStr()));

  // Generate Cascades
  TIntPrIntH EdgesUsed;
  int last = 0;

  for (int i=0; (i<NCascades) || ((double)EdgesUsed.Len() < -(double)NCascades/100.0 * (double)NetInfBs.GroundTruth->GetEdges()); i++) {
	  TCascade C;
	  NetInfBs.GenCascade(C, TModel, TimeHorizon, EdgesUsed, Delta);
	  NetInfBs.AddCasc(C);

	  if ( EdgesUsed.Len() - last > 100) {
		  last = EdgesUsed.Len();
		  printf("Cascades: %d, Edges used: %d/%f\n", NetInfBs.GetCascs(), EdgesUsed.Len(), (double)NetInfBs.GroundTruth->GetEdges());
	  }

	  switch(i) {
	  case 500:
		  NetInfBs.SaveCascades(TStr::Fmt("%s-500-cascades.txt", FileName.CStr()));
		  break;

	  case 1000:
		  NetInfBs.SaveCascades(TStr::Fmt("%s-1000-cascades.txt", FileName.CStr()));
		  break;

	  case 2000:
		  NetInfBs.SaveCascades(TStr::Fmt("%s-2000-cascades.txt", FileName.CStr()));
		  break;

	  case 5000:
		  NetInfBs.SaveCascades(TStr::Fmt("%s-5000-cascades.txt", FileName.CStr()));
		  break;

	  case 10000:
		  NetInfBs.SaveCascades(TStr::Fmt("%s-10000-cascades.txt", FileName.CStr()));
		  break;

	  default:
		  break;
	  }
  }

  printf("Cascades: %d, Edges used: %d/%f\n", NetInfBs.GetCascs(), EdgesUsed.Len(), (double)NetInfBs.GroundTruth->GetEdges());

  // Save Cascades
  NetInfBs.SaveCascades(TStr::Fmt("%s-cascades.txt", FileName.CStr()));

  Catch
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
}
