#include "TFile.h"
#include "TString.h"
#include "TH1F.h"

bool DEBUG = true;
bool DEBUG_pretag = true;
bool DEBUG_tag = false;
bool DEBUG_untag = false;
bool DEBUG_rebin = false;

void convert(std::vector<TString> fin, std::vector<TString> fout, std::vector<std::string> varName, std::string WP, double kScales[7], std::string kScaleStr)
{
   Float_t nReBin = 2.5;
//    Float_t nReBin = 1.;
   
   Float_t kScale_b = kScales[0];
   Float_t kScale_cfromg = kScales[1];
   Float_t kScale_c = kScales[2];
   Float_t kScale_l = kScales[3];

   Float_t kScale_b_cfromg = kScales[4];
   Float_t kScale_c_l = kScales[5];
   
   Float_t kScale = kScales[6];

   TFile *f;
   TString fout_;
   TFile *fNew;

   float int_total;
   std::vector<float> int_total_SYSup;
   std::vector<float> int_total_SYSdown;
   
   std::vector<float> k_lumi_allpt;
   std::vector<std::vector<float>> k_lumi_SYSup_allpt;
   std::vector<std::vector<float>> k_lumi_SYSdown_allpt;

   float k_lumi;
   std::vector<float> k_lumi_SYSup;
   std::vector<float> k_lumi_SYSdown;
   
   for(int ifile=0; ifile<fin.size();ifile++){

 	   std::cout << "" << std::endl; 
	   std::cout << "Processing to convert double-Mu-tag file ... " << fin.at(ifile) << std::endl; 
	      
	   f = TFile::Open(fin.at(ifile));
	   fout_ = fout.at(ifile)+"_DoubleB"+WP+kScaleStr+".root";
	   fNew = TFile::Open(fout_,"RECREATE");
	   std::cout<< "Creating file: "<< fout_ << std::endl;

	   std::string tag; 
	   tag = "DoubleB"+WP;
   
	   std::string var = varName.at(ifile);

	//    TString QCDlabel = "QCD";   
	   std::string QCDlabel = "QCDMuEnr";

	   const int npt = 17;
	   std::string ptt[npt] = {"pt250to300",
								"pt300to350",
								"pt350to400",
								"pt400to450",
								"pt450to500",
								"pt400to470",
								"pt470to2000",
								"pt350to430",
								"pt430to2000",
								"pt250to350",
								"pt350to450",
								"pt300to400",
								"pt400to500",
								"pt500to600",
								"pt600to2000",
								"pt500to2000",
								"pt450to2000",
								};
	  

	   const int nSys = 7;
	  std::string sysName[nSys] = {"JES","NTRACKS","BFRAG","CFRAG","CD","K0L","PU"};

	//    const int nSys=0;
	//    std::string sysName[nSys];


	   for(int i=0;i<npt;i++)
		 {
		std::string pt = ptt[i];
	
		// data
		std::string hname_data = "UNWEIGHTED__DATA__FatJet_"+var+"_all_"+pt+"_data";
		std::string hname_data_tag = "UNWEIGHTED__DATA__FatJet_"+var+"_"+tag+"pass_"+pt+"_data";
		std::string hname_data_untag = "UNWEIGHTED__DATA__FatJet_"+var+"_"+tag+"fail_"+pt+"_data";
		if(DEBUG)std::cout<< "attempt get data histograms "<< var << ", "<<tag <<", pt = "<<pt<< std::endl;
		if(DEBUG)std::cout<< hname_data << "XXX" <<std::endl;	
		TH1F *h_data = (TH1F*)f->Get(hname_data.c_str());
		TH1F *h_data_tag = (TH1F*)f->Get(hname_data_tag.c_str());
		TH1F *h_data_untag = (TH1F*)f->Get(hname_data_untag.c_str());
		if(DEBUG)std::cout<< "got data histograms."<<std::endl;	
		if(DEBUG)std::cout<< h_data <<std::endl;	
		float int_data = h_data->Integral(0,h_data->GetXaxis()->GetNbins()+1);
		float int_data_tag = h_data_tag->Integral(0,h_data_tag->GetXaxis()->GetNbins()+1);
		float int_data_untag = h_data_untag->Integral(0,h_data_untag->GetXaxis()->GetNbins()+1);

		// pre-tag
		if(i==8 && DEBUG_pretag)std::cout << "" << std::endl;
		if(i==8 && DEBUG_pretag)std::cout << "----------------------------------------" << std::endl;
		if(i==8 && DEBUG_pretag)std::cout << "PRETAG - "  << pt << std::endl;
		if(i==8 && DEBUG_pretag)std::cout << "----------------------------------------" << std::endl;
		std::string hname_bfromg = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_bfromg";
		std::string hname_b = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_b";
		std::string hname_cfromg = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_cfromg";
		std::string hname_c = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_c";
		std::string hname_l = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_l";

		TH1F *h_bfromg = (TH1F*)f->Get(hname_bfromg.c_str());
		if(i==8 && DEBUG_pretag)std::cout << "" << std::endl;	
		if(i==8 && DEBUG_pretag)std::cout << "before		:h_bfromg("<< hname_bfromg.c_str()<< ")->Integral() = "<< h_bfromg->Integral() << std::endl;
		h_bfromg->Scale(kScale);
		float int_bfromg = h_bfromg->Integral(0,h_bfromg->GetXaxis()->GetNbins()+1);
		TH1F *h_b = (TH1F*)f->Get(hname_b.c_str());
		h_b->Scale(kScale*kScale_b);
		float int_b = h_b->Integral(0,h_b->GetXaxis()->GetNbins()+1);
		TH1F *h_cfromg = (TH1F*)f->Get(hname_cfromg.c_str());
		h_cfromg->Scale(kScale*kScale_cfromg);
		float int_cfromg = h_cfromg->Integral(0,h_cfromg->GetXaxis()->GetNbins()+1);
		TH1F *h_c = (TH1F*)f->Get(hname_c.c_str());
		h_c->Scale(kScale*kScale_c);
		float int_c = h_c->Integral(0,h_c->GetXaxis()->GetNbins()+1);
		TH1F *h_l = (TH1F*)f->Get(hname_l.c_str());
		h_l->Scale(kScale*kScale_l);
		float int_l = h_l->Integral(0,h_l->GetXaxis()->GetNbins()+1);

		float int_total = int_bfromg+int_b+int_cfromg+int_c+int_l;
		//float k_lumi = int_data/int_total;
		//if(i==8 && DEBUG_pretag)std::cout << "k_lumi  = int_data/int_total =" << int_data <<"/"<<int_total << " = " << k_lumi << std::endl;

		if(varName.at(ifile)=="JP") if(i==8 && DEBUG_pretag)std::cout<<"varName="<<varName.at(ifile)<<std::endl;
		if(varName.at(ifile)=="JP") k_lumi_allpt.push_back(int_data/int_total);
		k_lumi = k_lumi_allpt.at(i);

		if(varName.at(ifile)=="JP") {if(i==8 && DEBUG_pretag)std::cout << "k_lumi  = int_data/int_total =" << int_data <<"/"<<int_total << " = " << k_lumi << std::endl;}
		else if(i==8 && DEBUG_pretag)std::cout << "k_lumi  = " << k_lumi << std::endl;

		h_bfromg->Scale(k_lumi);
		if(i==8 && DEBUG_pretag)std::cout << "after scale	:h_bfromg("<< hname_bfromg.c_str()<< ")->Integral() = "<< h_bfromg->Integral() << std::endl;
		h_b->Scale(k_lumi);
		h_cfromg->Scale(k_lumi);
		h_c->Scale(k_lumi);
		h_l->Scale(k_lumi);

	// 	float k_norm_bfromg = 1./h_bfromg->Integral();
	// 	if(i==8 && DEBUG_pretag)std::cout << "k_norm_bfromg  = "<< k_norm_bfromg << std::endl;
	// 	float k_norm_b = 1./h_b->Integral();
	// 	float k_norm_cfromg = 1./h_cfromg->Integral();
	// 	float k_norm_c = 1./h_c->Integral();
	// 	float k_norm_l = 1./h_l->Integral();
	// 
	// 	h_bfromg->Scale(k_norm_bfromg);
	// 	if(i==8 && DEBUG_pretag)std::cout << "after scale	:h_bfromg("<< hname_bfromg.c_str()<< ")->Integral() = "<< h_bfromg->Integral() << std::endl;
	// 	h_b->Scale(k_norm_b);
	// 	h_cfromg->Scale(k_norm_cfromg);
	// 	h_c->Scale(k_norm_c);
	// 	h_l->Scale(k_norm_l);


		TH1F *h_bfromg_JESup[100];
		TH1F *h_b_JESup[100];
		TH1F *h_cfromg_JESup[100];
		TH1F *h_c_JESup[100];
		TH1F *h_l_JESup[100];

		TH1F *h_bfromg_JESdown[100];
		TH1F *h_b_JESdown[100];
		TH1F *h_cfromg_JESdown[100];
		TH1F *h_c_JESdown[100];
		TH1F *h_l_JESdown[100];
		if(DEBUG)std::cout<< "CHECK A "<< std::endl;

		k_lumi_SYSup.clear(); //need new set of numbers every pt loop 
		k_lumi_SYSdown.clear(); //need new set of numbers every pt loop
	
		for(int is=0;is<nSys;is++)
		  {	    
			 // pre-tag SYS up
			 std::string hname_bfromg_JESup = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_all_"+sysName[is]+"up_"+pt+"_bfromg";
			 std::string hname_b_JESup = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_all_"+sysName[is]+"up_"+pt+"_b";
			 std::string hname_cfromg_JESup = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_all_"+sysName[is]+"up_"+pt+"_cfromg";
			 std::string hname_c_JESup = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_all_"+sysName[is]+"up_"+pt+"_c";
			 std::string hname_l_JESup = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_all_"+sysName[is]+"up_"+pt+"_l";
		 
			 if( sysName[is] == "NTRACKS" || sysName[is] == "CFRAG" || sysName[is] == "CD" || sysName[is] == "K0L" )
			   {
			  hname_bfromg_JESup = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_bfromg";
			  hname_b_JESup = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_b";
			  hname_cfromg_JESup = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_cfromg";
			  hname_c_JESup = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_c";
			  hname_l_JESup = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_l";
			   }	     

			 if( sysName[is] == "BFRAG" || sysName[is] == "PU" )
			   {
			  hname_bfromg_JESup = sysName[is]+"UP__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_bfromg";
			  hname_b_JESup = sysName[is]+"UP__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_b";
			  hname_cfromg_JESup = sysName[is]+"UP__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_cfromg";
			  hname_c_JESup = sysName[is]+"UP__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_c";
			  hname_l_JESup = sysName[is]+"UP__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_l";
			   }	     
		 
			 h_bfromg_JESup[is] = (TH1F*)f->Get(hname_bfromg_JESup.c_str());
			 if(i==8 && DEBUG_pretag)std::cout << "" << std::endl;
			 if(i==8 && DEBUG_pretag)std::cout << "before		:h_bfromg_JESup["<< is <<"]("<< hname_bfromg_JESup.c_str()<< ")->Integral() = "<< h_bfromg_JESup[is]->Integral() << std::endl;
			 h_bfromg_JESup[is]->Scale(kScale);
			 float int_bfromg_JESup = h_bfromg_JESup[is]->Integral(0,h_bfromg_JESup[is]->GetXaxis()->GetNbins()+1);
			 h_b_JESup[is] = (TH1F*)f->Get(hname_b_JESup.c_str());
			 h_b_JESup[is]->Scale(kScale);
			 float int_b_JESup = h_b_JESup[is]->Integral(0,h_b_JESup[is]->GetXaxis()->GetNbins()+1);
			 h_cfromg_JESup[is] = (TH1F*)f->Get(hname_cfromg_JESup.c_str());
			 h_cfromg_JESup[is]->Scale(kScale);
			 float int_cfromg_JESup = h_cfromg_JESup[is]->Integral(0,h_cfromg_JESup[is]->GetXaxis()->GetNbins()+1);
			 h_c_JESup[is] = (TH1F*)f->Get(hname_c_JESup.c_str());
			 h_c_JESup[is]->Scale(kScale);
			 float int_c_JESup = h_c_JESup[is]->Integral(0,h_c_JESup[is]->GetXaxis()->GetNbins()+1);
			 h_l_JESup[is] = (TH1F*)f->Get(hname_l_JESup.c_str());
			 h_l_JESup[is]->Scale(kScale);
			 float int_l_JESup = h_l_JESup[is]->Integral(0,h_l_JESup[is]->GetXaxis()->GetNbins()+1);
		 
			 float int_total_JESup = int_bfromg_JESup+int_b_JESup+int_cfromg_JESup+int_c_JESup+int_l_JESup;
			 float k_lumi_JESup = int_data/int_total_JESup;
			 if(i==8 && DEBUG_pretag)std::cout << "k_lumi_JESup  = int_data/int_total =" << int_data <<"/"<<int_total_JESup << " = " << k_lumi_JESup << std::endl;
		 
// 			 int_total_SYSup.push_back(int_total_JESup);
// 			 k_lumi_SYSup.push_back(k_lumi_JESup);
 
			 if(varName.at(ifile)=="JP"){ 
			 	k_lumi_SYSup.push_back(k_lumi_JESup); 
			 	if(DEBUG)std::cout << "Im saving k_lumi_JESup(="<< k_lumi_JESup<<") in k_lumi_SYSup.at("<< is<<") = "<< k_lumi_SYSup.at(is) << std::endl;
			 	if(DEBUG)std::cout << "k_lumi_SYSup.at("<< is <<")  = " << k_lumi_SYSup.at(is) << std::endl;
			 }
			 else{
				 if(DEBUG)std::cout << "k_lumi_SYSup_allpt.size()  = " << k_lumi_SYSup_allpt.size() << std::endl;
				 if(DEBUG)std::cout << "k_lumi_SYSup_allpt.at("<< i <<").size()  = " << k_lumi_SYSup_allpt.at(i).size() << std::endl;
				 if(DEBUG)std::cout << "k_lumi_SYSup_allpt.at("<< i <<").at("<< is <<")  = " << k_lumi_SYSup_allpt.at(i).at(is) << std::endl;
				 k_lumi_JESup = k_lumi_SYSup_allpt.at(i).at(is); //overrides and take value of norm of pretag JP 
			 }
		 
			 h_bfromg_JESup[is]->Scale(k_lumi_JESup);
			 if(i==8 && DEBUG_pretag)std::cout << "after scale	:h_bfromg_JESup["<< is <<"]("<< hname_bfromg_JESup.c_str()<< ")->Integral() = "<< h_bfromg_JESup[is]->Integral() << std::endl;
			 h_b_JESup[is]->Scale(k_lumi_JESup);
			 h_cfromg_JESup[is]->Scale(k_lumi_JESup);
			 h_c_JESup[is]->Scale(k_lumi_JESup);
			 h_l_JESup[is]->Scale(k_lumi_JESup);

	// 		k_norm_bfromg = 1./h_bfromg_JESup[is]->Integral();
	// 		if(i==8 && DEBUG_pretag)std::cout << "k_norm_bfromg_JESup["<<is<<"]  = "<< k_norm_bfromg << std::endl;
	// 		k_norm_b = 1./h_b_JESup[is]->Integral();
	// 		k_norm_cfromg = 1./h_cfromg_JESup[is]->Integral();
	// 		k_norm_c = 1./h_c_JESup[is]->Integral();
	// 		k_norm_l = 1./h_l_JESup[is]->Integral();
	// 
	// 	     h_bfromg_JESup[is]->Scale(k_norm_bfromg);
	// 	     if(i==8 && DEBUG_pretag)std::cout << "after scale	:h_bfromg_JESup["<< is <<"]("<< hname_bfromg_JESup.c_str()<< ")->Integral() = "<< h_bfromg_JESup[is]->Integral() << std::endl;
	// 	     h_b_JESup[is]->Scale(k_norm_b);
	// 	     h_cfromg_JESup[is]->Scale(k_norm_cfromg);
	// 	     h_c_JESup[is]->Scale(k_norm_c);
	// 	     h_l_JESup[is]->Scale(k_norm_l);


			 // pre-tag JESdown
			 std::string hname_bfromg_JESdown = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_all_"+sysName[is]+"down_"+pt+"_bfromg";
			 std::string hname_b_JESdown = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_all_"+sysName[is]+"down_"+pt+"_b";
			 std::string hname_cfromg_JESdown = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_all_"+sysName[is]+"down_"+pt+"_cfromg";
			 std::string hname_c_JESdown = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_all_"+sysName[is]+"down_"+pt+"_c";
			 std::string hname_l_JESdown = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_all_"+sysName[is]+"down_"+pt+"_l";

			 if( sysName[is] == "NTRACKS" || sysName[is] == "CFRAG" || sysName[is] == "CD" || sysName[is] == "K0L" )
			   {
				  hname_bfromg_JESdown = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_bfromg";
				  hname_b_JESdown = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_b";
				  hname_cfromg_JESdown = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_cfromg";
				  hname_c_JESdown = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_c";
				  hname_l_JESdown = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_l";
			   }	     

			 if( sysName[is] == "BFRAG" || sysName[is] == "PU" )
			   {
				  hname_bfromg_JESdown = sysName[is]+"DOWN__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_bfromg";
				  hname_b_JESdown = sysName[is]+"DOWN__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_b";
				  hname_cfromg_JESdown = sysName[is]+"DOWN__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_cfromg";
				  hname_c_JESdown = sysName[is]+"DOWN__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_c";
				  hname_l_JESdown = sysName[is]+"DOWN__"+QCDlabel+"__FatJet_"+var+"_all_"+pt+"_l";
			   }	     
		 
			 h_bfromg_JESdown[is] = (TH1F*)f->Get(hname_bfromg_JESdown.c_str());
			 if(i==8 && DEBUG_pretag)std::cout << "" << std::endl;
			 if(i==8 && DEBUG_pretag)std::cout << "before		:h_bfromg_JESdown["<< is <<"]("<< hname_bfromg_JESdown.c_str()<< ")->Integral() = "<< h_bfromg_JESdown[is]->Integral() << std::endl;
			 h_bfromg_JESdown[is]->Scale(kScale);
			 float int_bfromg_JESdown = h_bfromg_JESdown[is]->Integral(0,h_bfromg_JESdown[is]->GetXaxis()->GetNbins()+1);
			 h_b_JESdown[is] = (TH1F*)f->Get(hname_b_JESdown.c_str());
			 h_b_JESdown[is]->Scale(kScale);
			 float int_b_JESdown = h_b_JESdown[is]->Integral(0,h_b_JESdown[is]->GetXaxis()->GetNbins()+1);
			 h_cfromg_JESdown[is] = (TH1F*)f->Get(hname_cfromg_JESdown.c_str());
			 h_cfromg_JESdown[is]->Scale(kScale);
			 float int_cfromg_JESdown = h_cfromg_JESdown[is]->Integral(0,h_cfromg_JESdown[is]->GetXaxis()->GetNbins()+1);
			 h_c_JESdown[is] = (TH1F*)f->Get(hname_c_JESdown.c_str());
			 h_c_JESdown[is]->Scale(kScale);
			 float int_c_JESdown = h_c_JESdown[is]->Integral(0,h_c_JESdown[is]->GetXaxis()->GetNbins()+1);
			 h_l_JESdown[is] = (TH1F*)f->Get(hname_l_JESdown.c_str());
			 h_l_JESdown[is]->Scale(kScale);
			 float int_l_JESdown = h_l_JESdown[is]->Integral(0,h_l_JESdown[is]->GetXaxis()->GetNbins()+1);
		 
			 float int_total_JESdown = int_bfromg_JESdown+int_b_JESdown+int_cfromg_JESdown+int_c_JESdown+int_l_JESdown;
			 float k_lumi_JESdown = int_data/int_total_JESdown;
			 if(i==8 && DEBUG_pretag)std::cout << "k_lumi_JESdown  = int_data/int_total =" << int_data <<"/"<<int_total_JESdown << " = " << k_lumi_JESdown << std::endl;

// 			 int_total_SYSdown.push_back(int_total_JESdown);
// 			 k_lumi_SYSdown.push_back(k_lumi_JESdown);

			 if(varName.at(ifile)=="JP") k_lumi_SYSdown.push_back(k_lumi_JESdown);
			 else k_lumi_JESdown = k_lumi_SYSdown_allpt.at(i).at(is); //overrides and take value of norm of pretag JP 
		 
			 if( sysName[is] == "BFRAG" || sysName[is] == "PU" || sysName[is] == "JES" ){ //because only these sys have "down"s other sys are copies of their "up"s.
				 h_bfromg_JESdown[is]->Scale(k_lumi_JESdown);
				 if(i==8 && DEBUG_pretag)std::cout << "after scale	:h_bfromg_JESdown["<< is <<"]("<< hname_bfromg_JESdown.c_str()<< ")->Integral() = "<< h_bfromg_JESdown[is]->Integral() << std::endl;
				 h_b_JESdown[is]->Scale(k_lumi_JESdown);
				 h_cfromg_JESdown[is]->Scale(k_lumi_JESdown);
				 h_c_JESdown[is]->Scale(k_lumi_JESdown);
				 h_l_JESdown[is]->Scale(k_lumi_JESdown);
				}

	// 		k_norm_bfromg = 1./h_bfromg_JESdown[is]->Integral();
	// 		if(i==8 && DEBUG_pretag)std::cout << "k_norm_bfromg_JESdown["<<is<<"]  = "<< k_norm_bfromg << std::endl;
	// 		k_norm_b = 1./h_b_JESdown[is]->Integral();
	// 		k_norm_cfromg = 1./h_cfromg_JESdown[is]->Integral();
	// 		k_norm_c = 1./h_c_JESdown[is]->Integral();
	// 		k_norm_l = 1./h_l_JESdown[is]->Integral();
	// 
	// 	     h_bfromg_JESdown[is]->Scale(k_norm_bfromg);
	// 	     if(i==8 && DEBUG_pretag)std::cout << "after scale	:h_bfromg_JESdown["<< is <<"]("<< hname_bfromg_JESdown.c_str()<< ")->Integral() = "<< h_bfromg_JESdown[is]->Integral() << std::endl;
	// 	     h_b_JESdown[is]->Scale(k_norm_b);
	// 	     h_cfromg_JESdown[is]->Scale(k_norm_cfromg);
	// 	     h_c_JESdown[is]->Scale(k_norm_c);
	// 	     h_l_JESdown[is]->Scale(k_norm_l);

		  }	

		if(varName.at(ifile)=="JP") k_lumi_SYSup_allpt.push_back(k_lumi_SYSup);
		if(varName.at(ifile)=="JP") k_lumi_SYSdown_allpt.push_back(k_lumi_SYSdown);

		if(DEBUG)std::cout<< "CHECK B "<< std::endl;

		// tag
		if(i==8 && DEBUG_tag)std::cout << "" << std::endl;
		if(i==8 && DEBUG_tag)std::cout << "----------------------------------------" << std::endl;
		if(i==8 && DEBUG_tag)std::cout << "TAG - " << pt <<std::endl;
		if(i==8 && DEBUG_tag)std::cout << "----------------------------------------" << std::endl;
		std::string hname_bfromg_tag = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_bfromg";
		std::string hname_b_tag = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_b";
		std::string hname_cfromg_tag = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_cfromg";
		std::string hname_c_tag = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_c";
		std::string hname_l_tag = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_l";
	
		TH1F *h_bfromg_tag = (TH1F*)f->Get(hname_bfromg_tag.c_str());	
		if(i==8 && DEBUG_tag)std::cout << "" << std::endl;
		if(i==8 && DEBUG_tag)std::cout << "before		:h_bfromg_tag("<< hname_bfromg_tag.c_str()<< ")->Integral() = "<< h_bfromg_tag->Integral() << std::endl;
		h_bfromg_tag->Scale(kScale);
		TH1F *h_b_tag = (TH1F*)f->Get(hname_b_tag.c_str());
		h_b_tag->Scale(kScale*kScale_b);
		TH1F *h_cfromg_tag = (TH1F*)f->Get(hname_cfromg_tag.c_str());
		h_cfromg_tag->Scale(kScale*kScale_cfromg);
		TH1F *h_c_tag = (TH1F*)f->Get(hname_c_tag.c_str());
		h_c_tag->Scale(kScale*kScale_c);
		TH1F *h_l_tag = (TH1F*)f->Get(hname_l_tag.c_str());
		h_l_tag->Scale(kScale*kScale_l);
	
	// 	int_bfromg = h_bfromg_tag->Integral(0,h_bfromg_tag->GetXaxis()->GetNbins()+1);
	// 	int_b = h_b_tag->Integral(0,h_b_tag->GetXaxis()->GetNbins()+1);
	// 	int_cfromg = h_cfromg_tag->Integral(0,h_cfromg_tag->GetXaxis()->GetNbins()+1);
	// 	int_c = h_c_tag->Integral(0,h_c_tag->GetXaxis()->GetNbins()+1);
	// 	int_l = h_l_tag->Integral(0,h_l_tag->GetXaxis()->GetNbins()+1);
	// 
	// 	int_total = int_bfromg+int_b+int_cfromg+int_c+int_l;
	// 	k_lumi = int_data_tag/int_total;
	// 	if(i==8 && DEBUG_tag)std::cout << "k_lumi  = int_data_tag/int_total =" << int_data_tag <<"/"<<int_total << " = " << k_lumi << std::endl;
// 		if(i==8 && DEBUG_tag)std::cout << "k_lumi  = int_data/int_total =" << int_data <<"/"<<int_total << " = " << k_lumi << std::endl;
		if(i==8 && DEBUG_tag)std::cout << "k_lumi  = " << k_lumi << std::endl;

		h_bfromg_tag->Scale(k_lumi);
		if(i==8 && DEBUG_tag)std::cout << "after scale	:h_bfromg_tag("<< hname_bfromg_tag.c_str()<< ")->Integral() = "<< h_bfromg_tag->Integral() << std::endl;
		h_b_tag->Scale(k_lumi);
		h_cfromg_tag->Scale(k_lumi);
		h_c_tag->Scale(k_lumi);
		h_l_tag->Scale(k_lumi);

	// 	float k_norm_bfromg_tag = 1./h_bfromg_tag->Integral();
	// 	if(i==8 && DEBUG_tag)std::cout << "k_norm_bfromg  = "<< k_norm_bfromg_tag << std::endl;
	// 	float k_norm_b_tag = 1./h_b_tag->Integral();
	// 	float k_norm_cfromg_tag = 1./h_cfromg_tag->Integral();
	// 	float k_norm_c_tag = 1./h_c_tag->Integral();
	// 	float k_norm_l_tag = 1./h_l_tag->Integral();

	// 	h_bfromg_tag->Scale(k_norm_bfromg_tag);
	// 	if(i==8 && DEBUG_tag)std::cout << "after scale	:h_bfromg_tag("<< hname_bfromg_tag.c_str()<< ")->Integral() = "<< h_bfromg_tag->Integral() << std::endl;
	// 	h_b_tag->Scale(k_norm_b_tag);
	// 	h_cfromg_tag->Scale(k_norm_cfromg_tag);
	// 	h_c_tag->Scale(k_norm_c_tag);
	// 	h_l_tag->Scale(k_norm_l_tag);

		TH1F *h_bfromg_tag_JESup[100];
		TH1F *h_b_tag_JESup[100];
		TH1F *h_cfromg_tag_JESup[100];
		TH1F *h_c_tag_JESup[100];
		TH1F *h_l_tag_JESup[100];

		TH1F *h_bfromg_tag_JESdown[100];
		TH1F *h_b_tag_JESdown[100];
		TH1F *h_cfromg_tag_JESdown[100];
		TH1F *h_c_tag_JESdown[100];
		TH1F *h_l_tag_JESdown[100];
		if(DEBUG)std::cout<< "CHECK C "<< std::endl;
	
		for(int is=0;is<nSys;is++)
		  {	    	
			 // tag JESup
			 std::string hname_bfromg_tag_JESup = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_"+sysName[is]+"up_bfromg";
			 std::string hname_b_tag_JESup = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_"+sysName[is]+"up_b";
			 std::string hname_cfromg_tag_JESup = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_"+sysName[is]+"up_cfromg";
			 std::string hname_c_tag_JESup = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_"+sysName[is]+"up_c";
			 std::string hname_l_tag_JESup = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_"+sysName[is]+"up_l";

			 if( sysName[is] == "NTRACKS" || sysName[is] == "CFRAG" || sysName[is] == "CD" || sysName[is] == "K0L" )
			   {
				  hname_bfromg_tag_JESup = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_bfromg";
				  hname_b_tag_JESup = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_b";
				  hname_cfromg_tag_JESup = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_cfromg";
				  hname_c_tag_JESup = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_c";
				  hname_l_tag_JESup = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_l";
			   }	     

			 if( sysName[is] == "BFRAG" || sysName[is] == "PU" )
			   {
				  hname_bfromg_tag_JESup = sysName[is]+"UP__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_bfromg";
				  hname_b_tag_JESup = sysName[is]+"UP__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_b";
				  hname_cfromg_tag_JESup = sysName[is]+"UP__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_cfromg";
				  hname_c_tag_JESup = sysName[is]+"UP__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_c";
				  hname_l_tag_JESup = sysName[is]+"UP__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_l";
			   }	     
		 
			 h_bfromg_tag_JESup[is] = (TH1F*)f->Get(hname_bfromg_tag_JESup.c_str());
			 if(i==8 && DEBUG_tag)std::cout << "" << std::endl;
			 if(i==8 && DEBUG_tag)std::cout << "before		:h_bfromg_tag_JESup["<<is<<"]("<< hname_bfromg_tag_JESup.c_str()<< ")->Integral() = "<< h_bfromg_tag_JESup[is]->Integral() << std::endl;
			 h_bfromg_tag_JESup[is]->Scale(kScale);
			 float int_bfromg_tag_JESup = h_bfromg_tag_JESup[is]->Integral(0,h_bfromg_tag_JESup[is]->GetXaxis()->GetNbins()+1);
			 h_b_tag_JESup[is] = (TH1F*)f->Get(hname_b_tag_JESup.c_str());
			 h_b_tag_JESup[is]->Scale(kScale);
			 float int_b_tag_JESup = h_b_tag_JESup[is]->Integral(0,h_b_tag_JESup[is]->GetXaxis()->GetNbins()+1);
			 h_cfromg_tag_JESup[is] = (TH1F*)f->Get(hname_cfromg_tag_JESup.c_str());
			 h_cfromg_tag_JESup[is]->Scale(kScale);
			 float int_cfromg_tag_JESup = h_cfromg_tag_JESup[is]->Integral(0,h_cfromg_tag_JESup[is]->GetXaxis()->GetNbins()+1);
			 h_c_tag_JESup[is] = (TH1F*)f->Get(hname_c_tag_JESup.c_str());
			 h_c_tag_JESup[is]->Scale(kScale);
			 float int_c_tag_JESup = h_c_tag_JESup[is]->Integral(0,h_c_tag_JESup[is]->GetXaxis()->GetNbins()+1);
			 h_l_tag_JESup[is] = (TH1F*)f->Get(hname_l_tag_JESup.c_str());
			 h_l_tag_JESup[is]->Scale(kScale);
			 float int_l_tag_JESup = h_l_tag_JESup[is]->Integral(0,h_l_tag_JESup[is]->GetXaxis()->GetNbins()+1);
		 
	//      float int_total_tag_JESup = int_bfromg_tag_JESup+int_b_tag_JESup+int_cfromg_tag_JESup+int_c_tag_JESup+int_l_tag_JESup;
	//      float k_lumi_tag_JESup = int_data_tag/int_total_tag_JESup;
	//	 float int_total_tag_JESup = int_total_SYSup.at(is);
// 		 float k_lumi_tag_JESup = k_lumi_SYSup.at(is);
			
			 float k_lumi_tag_JESup;
// 			 if(varName.at(ifile)=="JP") k_lumi_tag_JESup = k_lumi_SYSup.at(is);
// 			 else k_lumi_tag_JESup = k_lumi_SYSup_allpt.at(i).at(is); //overrides and take value of norm of pretag JP 
			 if(i==8 && DEBUG_tag)std::cout << "k_lumi_SYSup_allpt.size()  = " << k_lumi_SYSup_allpt.size() << std::endl;
			 if(i==8 && DEBUG_tag)std::cout << "k_lumi_SYSup_allpt.at("<< i <<").size()  = " << k_lumi_SYSup_allpt.at(i).size() << std::endl;
			 k_lumi_tag_JESup = k_lumi_SYSup_allpt.at(i).at(is); //overrides and take value of norm of pretag JP 
			 if(i==8 && DEBUG_tag)std::cout << "k_lumi_tag_JESup  = " << k_lumi_tag_JESup << std::endl;
		 
			 h_bfromg_tag_JESup[is]->Scale(k_lumi_tag_JESup);
			 if(i==8 && DEBUG_tag)std::cout << "after scale	:h_bfromg_tag_JESup["<< is <<"]("<< hname_bfromg_tag_JESup.c_str()<< ")->Integral() = "<< h_bfromg_tag_JESup[is]->Integral() << std::endl;
			 h_b_tag_JESup[is]->Scale(k_lumi_tag_JESup);
			 h_cfromg_tag_JESup[is]->Scale(k_lumi_tag_JESup);
			 h_c_tag_JESup[is]->Scale(k_lumi_tag_JESup);
			 h_l_tag_JESup[is]->Scale(k_lumi_tag_JESup);

	// 		k_norm_bfromg_tag = 1./h_bfromg_tag_JESup[is]->Integral();
	// 		if(i==8 && DEBUG_tag)std::cout << "k_norm_bfromg_JESup["<<is<<"]  = "<< k_norm_bfromg << std::endl;
	// 		k_norm_b_tag = 1./h_b_tag_JESup[is]->Integral();
	// 		k_norm_cfromg_tag = 1./h_cfromg_tag_JESup[is]->Integral();
	// 		k_norm_c_tag = 1./h_c_tag_JESup[is]->Integral();
	// 		k_norm_l_tag = 1./h_l_tag_JESup[is]->Integral();
	// 
	// 	     h_bfromg_tag_JESup[is]->Scale(k_norm_bfromg_tag);
	// 	     if(i==8 && DEBUG_tag)std::cout << "after scale	:h_bfromg_tag_JESup["<< is <<"]("<< hname_bfromg_tag_JESup.c_str()<< ")->Integral() = "<< h_bfromg_tag_JESup[is]->Integral() << std::endl;
	// 	     h_b_tag_JESup[is]->Scale(k_norm_b_tag);
	// 	     h_cfromg_tag_JESup[is]->Scale(k_norm_cfromg_tag);
	// 	     h_c_tag_JESup[is]->Scale(k_norm_c_tag);
	// 	     h_l_tag_JESup[is]->Scale(k_norm_l_tag);

		 
			 // tag JESdown
			 std::string hname_bfromg_tag_JESdown = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_"+sysName[is]+"down_bfromg";
			 std::string hname_b_tag_JESdown = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_"+sysName[is]+"down_b";
			 std::string hname_cfromg_tag_JESdown = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_"+sysName[is]+"down_cfromg";
			 std::string hname_c_tag_JESdown = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_"+sysName[is]+"down_c";
			 std::string hname_l_tag_JESdown = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_"+sysName[is]+"down_l";

			 if( sysName[is] == "NTRACKS" || sysName[is] == "CFRAG" || sysName[is] == "CD" || sysName[is] == "K0L" )
			   {
				  hname_bfromg_tag_JESdown = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_bfromg";
				  hname_b_tag_JESdown = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_b";
				  hname_cfromg_tag_JESdown = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_cfromg";
				  hname_c_tag_JESdown = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_c";
				  hname_l_tag_JESdown = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_l";
			   }

			 if( sysName[is] == "BFRAG" || sysName[is] == "PU" )
			   {
				  hname_bfromg_tag_JESdown = sysName[is]+"DOWN__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_bfromg";
				  hname_b_tag_JESdown = sysName[is]+"DOWN__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_b";
				  hname_cfromg_tag_JESdown = sysName[is]+"DOWN__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_cfromg";
				  hname_c_tag_JESdown = sysName[is]+"DOWN__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_c";
				  hname_l_tag_JESdown = sysName[is]+"DOWN__"+QCDlabel+"__FatJet_"+var+"_"+tag+"pass_"+pt+"_l";
			   }
		 
			 h_bfromg_tag_JESdown[is] = (TH1F*)f->Get(hname_bfromg_tag_JESdown.c_str());
			 if(i==8 && DEBUG_tag)std::cout << "" << std::endl;
			 if(i==8 && DEBUG_tag)std::cout << "before		:h_bfromg_tag_JESdown["<< is <<"]("<< hname_bfromg_tag_JESdown.c_str()<< ")->Integral() = "<< h_bfromg_tag_JESdown[is]->Integral() << std::endl;
			 h_bfromg_tag_JESdown[is]->Scale(kScale);
			 float int_bfromg_tag_JESdown = h_bfromg_tag_JESdown[is]->Integral(0,h_bfromg_tag_JESdown[is]->GetXaxis()->GetNbins()+1);
			 h_b_tag_JESdown[is] = (TH1F*)f->Get(hname_b_tag_JESdown.c_str());
			 h_b_tag_JESdown[is]->Scale(kScale);
			 float int_b_tag_JESdown = h_b_tag_JESdown[is]->Integral(0,h_b_tag_JESdown[is]->GetXaxis()->GetNbins()+1);
			 h_cfromg_tag_JESdown[is] = (TH1F*)f->Get(hname_cfromg_tag_JESdown.c_str());
			 h_cfromg_tag_JESdown[is]->Scale(kScale);
			 float int_cfromg_tag_JESdown = h_cfromg_tag_JESdown[is]->Integral(0,h_cfromg_tag_JESdown[is]->GetXaxis()->GetNbins()+1);
			 h_c_tag_JESdown[is] = (TH1F*)f->Get(hname_c_tag_JESdown.c_str());
			 h_c_tag_JESdown[is]->Scale(kScale);
			 float int_c_tag_JESdown = h_c_tag_JESdown[is]->Integral(0,h_c_tag_JESdown[is]->GetXaxis()->GetNbins()+1);
			 h_l_tag_JESdown[is] = (TH1F*)f->Get(hname_l_tag_JESdown.c_str());
			 h_l_tag_JESdown[is]->Scale(kScale);
			 float int_l_tag_JESdown = h_l_tag_JESdown[is]->Integral(0,h_l_tag_JESdown[is]->GetXaxis()->GetNbins()+1);
		 
	//      float int_total_tag_JESdown = int_bfromg_tag_JESdown+int_b_tag_JESdown+int_cfromg_tag_JESdown+int_c_tag_JESdown+int_l_tag_JESdown;
	//      float k_lumi_tag_JESdown = int_data_tag/int_total_tag_JESdown;
	//	 float int_total_tag_JESdown = int_total_SYSdown.at(is);
	//	 float k_lumi_tag_JESdown = k_lumi_SYSdown.at(is);

			 float k_lumi_tag_JESdown;
// 			 if(varName.at(ifile)=="JP") k_lumi_tag_JESdown = k_lumi_SYSdown.at(is);
// 			 else k_lumi_tag_JESdown = k_lumi_SYSdown_allpt.at(i).at(is); //overrides and take value of norm of pretag JP 
			 k_lumi_tag_JESdown = k_lumi_SYSdown_allpt.at(i).at(is); //overrides and take value of norm of pretag JP 
			 if(i==8 && DEBUG_tag)std::cout << "k_lumi_tag_JESdown  = " << k_lumi_tag_JESdown << std::endl;
		 
			 if( sysName[is] == "BFRAG" || sysName[is] == "PU" || sysName[is] == "JES" ){ //because only these sys have "down"s other sys are copies of their "up"s.
				 h_bfromg_tag_JESdown[is]->Scale(k_lumi_tag_JESdown);
				 if(i==8 && DEBUG_tag)std::cout << "after scale	:h_bfromg_tag_JESdown["<< is <<"]("<< hname_bfromg_tag_JESdown.c_str()<< ")->Integral() = "<< h_bfromg_tag_JESdown[is]->Integral() << std::endl;
				 h_b_tag_JESdown[is]->Scale(k_lumi_tag_JESdown);
				 h_cfromg_tag_JESdown[is]->Scale(k_lumi_tag_JESdown);
				 h_c_tag_JESdown[is]->Scale(k_lumi_tag_JESdown);
				 h_l_tag_JESdown[is]->Scale(k_lumi_tag_JESdown);
			 } 

	// 		k_norm_bfromg_tag = 1./h_bfromg_tag_JESdown[is]->Integral();
	// 		if(i==8 && DEBUG_tag)std::cout << "k_norm_bfromg_tag_JESdown["<<is<<"]  = "<< k_norm_bfromg_tag << std::endl;
	// 		k_norm_b_tag = 1./h_b_tag_JESdown[is]->Integral();
	// 		k_norm_cfromg_tag = 1./h_cfromg_tag_JESdown[is]->Integral();
	// 		k_norm_c_tag = 1./h_c_tag_JESdown[is]->Integral();
	// 		k_norm_l_tag = 1./h_l_tag_JESdown[is]->Integral();
	// 
	// 	     h_bfromg_tag_JESdown[is]->Scale(k_norm_bfromg_tag);
	// 	     if(i==8 && DEBUG_tag)std::cout << "after scale	:h_bfromg_tag_JESdown["<< is <<"]("<< hname_bfromg_tag_JESdown.c_str()<< ")->Integral() = "<< h_bfromg_tag_JESdown[is]->Integral() << std::endl;
	// 	     h_b_tag_JESdown[is]->Scale(k_norm_b_tag);
	// 	     h_cfromg_tag_JESdown[is]->Scale(k_norm_cfromg_tag);
	// 	     h_c_tag_JESdown[is]->Scale(k_norm_c_tag);
	// 	     h_l_tag_JESdown[is]->Scale(k_norm_l_tag);
		  }	
		if(DEBUG)std::cout<< "CHECK D "<< std::endl;

		// untag
		if(i==8 && DEBUG_untag)std::cout << "" << std::endl;
		if(i==8 && DEBUG_untag)std::cout << "----------------------------------------" << std::endl;
		if(i==8 && DEBUG_untag)std::cout << "UNTAG - " << pt <<std::endl;
		if(i==8 && DEBUG_untag)std::cout << "----------------------------------------" << std::endl;
		std::string hname_bfromg_untag = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_bfromg";
		std::string hname_b_untag = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_b";
		std::string hname_cfromg_untag = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_cfromg";
		std::string hname_c_untag = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_c";
		std::string hname_l_untag = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_l";
	
		TH1F *h_bfromg_untag = (TH1F*)f->Get(hname_bfromg_untag.c_str());
		if(i==8 && DEBUG_untag)std::cout << "" << std::endl;
		if(i==8 && DEBUG_untag)std::cout << "before		:h_bfromg_untag("<< hname_bfromg_untag.c_str()<< ")->Integral() = "<< h_bfromg_untag->Integral() << std::endl;
		h_bfromg_untag->Scale(kScale);
		TH1F *h_b_untag = (TH1F*)f->Get(hname_b_untag.c_str());
		h_b_untag->Scale(kScale*kScale_b);
		TH1F *h_cfromg_untag = (TH1F*)f->Get(hname_cfromg_untag.c_str());
		h_cfromg_untag->Scale(kScale*kScale_cfromg);
		TH1F *h_c_untag = (TH1F*)f->Get(hname_c_untag.c_str());
		h_c_untag->Scale(kScale*kScale_c);
		TH1F *h_l_untag = (TH1F*)f->Get(hname_l_untag.c_str());
		h_l_untag->Scale(kScale*kScale_l);

	// 	int_bfromg = h_bfromg_untag->Integral(0,h_bfromg_untag->GetXaxis()->GetNbins()+1);
	// 	int_b = h_b_untag->Integral(0,h_b_untag->GetXaxis()->GetNbins()+1);
	// 	int_cfromg = h_cfromg_untag->Integral(0,h_cfromg_untag->GetXaxis()->GetNbins()+1);
	// 	int_c = h_c_untag->Integral(0,h_c_untag->GetXaxis()->GetNbins()+1);
	// 	int_l = h_l_untag->Integral(0,h_l_untag->GetXaxis()->GetNbins()+1);
	// 
	// 	int_total = int_bfromg+int_b+int_cfromg+int_c+int_l;
	// 	k_lumi = int_data_untag/int_total;
	// 	if(i==8 && DEBUG_untag)std::cout << "k_lumi  = int_data_untag/int_total =" << int_data_untag <<"/"<<int_total << " = " << k_lumi << std::endl;
// 		if(i==8 && DEBUG_untag)std::cout << "k_lumi  = int_data/int_total =" << int_data <<"/"<<int_total << " = " << k_lumi << std::endl;
		if(i==8 && DEBUG_untag)std::cout << "k_lumi  = " << k_lumi << std::endl;

		h_bfromg_untag->Scale(k_lumi);
		if(i==8 && DEBUG_untag)std::cout << "after scale	:h_bfromg_untag("<< hname_bfromg_untag.c_str()<< ")->Integral() = "<< h_bfromg_untag->Integral() << std::endl;
		h_b_untag->Scale(k_lumi);
		h_cfromg_untag->Scale(k_lumi);
		h_c_untag->Scale(k_lumi);
		h_l_untag->Scale(k_lumi);

	// 	float k_norm_bfromg_untag = 1./h_bfromg_untag->Integral();
	// 	if(i==8 && DEBUG_untag)std::cout << "k_norm_bfromg  = "<< k_norm_bfromg_untag << std::endl;
	// 	float k_norm_b_untag = 1./h_b_untag->Integral();
	// 	float k_norm_cfromg_untag = 1./h_cfromg_untag->Integral();
	// 	float k_norm_c_untag = 1./h_c_untag->Integral();
	// 	float k_norm_l_untag = 1./h_l_untag->Integral();
	// 
	// 	h_bfromg_untag->Scale(k_norm_bfromg_untag);
	// 	if(i==8 && DEBUG_untag)std::cout << "after	scale	:h_bfromg_untag("<< hname_bfromg_untag.c_str()<< ")->Integral() = "<< h_bfromg_untag->Integral() << std::endl;
	// 	h_b_untag->Scale(k_norm_b_untag);
	// 	h_cfromg_untag->Scale(k_norm_cfromg_untag);
	// 	h_c_untag->Scale(k_norm_c_untag);
	// 	h_l_untag->Scale(k_norm_l_untag);


		TH1F *h_bfromg_untag_JESup[100];
		TH1F *h_b_untag_JESup[100];
		TH1F *h_cfromg_untag_JESup[100];
		TH1F *h_c_untag_JESup[100];
		TH1F *h_l_untag_JESup[100];

		TH1F *h_bfromg_untag_JESdown[100];
		TH1F *h_b_untag_JESdown[100];
		TH1F *h_cfromg_untag_JESdown[100];
		TH1F *h_c_untag_JESdown[100];
		TH1F *h_l_untag_JESdown[100];
		if(DEBUG)std::cout<< "CHECK E "<< std::endl;
	
		for(int is=0;is<nSys;is++)
		  {	    	
			 // untag JESup
			 std::string hname_bfromg_untag_JESup = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_"+sysName[is]+"up_bfromg";
			 std::string hname_b_untag_JESup = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_"+sysName[is]+"up_b";
			 std::string hname_cfromg_untag_JESup = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_"+sysName[is]+"up_cfromg";
			 std::string hname_c_untag_JESup = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_"+sysName[is]+"up_c";
			 std::string hname_l_untag_JESup = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_"+sysName[is]+"up_l";

			 if( sysName[is] == "NTRACKS" || sysName[is] == "CFRAG" || sysName[is] == "CD" || sysName[is] == "K0L" )
			   {
				  hname_bfromg_untag_JESup = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_bfromg";
				  hname_b_untag_JESup = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_b";
				  hname_cfromg_untag_JESup = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_cfromg";
				  hname_c_untag_JESup = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_c";
				  hname_l_untag_JESup = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_l";
			   }	     

			 if( sysName[is] == "BFRAG" || sysName[is] == "PU" )
			   {
				  hname_bfromg_untag_JESup = sysName[is]+"UP__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_bfromg";
				  hname_b_untag_JESup = sysName[is]+"UP__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_b";
				  hname_cfromg_untag_JESup = sysName[is]+"UP__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_cfromg";
				  hname_c_untag_JESup = sysName[is]+"UP__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_c";
				  hname_l_untag_JESup = sysName[is]+"UP__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_l";
			   }	     
		 
			 h_bfromg_untag_JESup[is] = (TH1F*)f->Get(hname_bfromg_untag_JESup.c_str());
			 if(i==8 && DEBUG_untag)std::cout << "" << std::endl;
			 if(i==8 && DEBUG_untag)std::cout << "before		:h_bfromg_untag_JESup["<< is <<"]("<< hname_bfromg_untag_JESup.c_str()<< ")->Integral() = "<< h_bfromg_untag_JESup[is]->Integral() << std::endl;
			 h_bfromg_untag_JESup[is]->Scale(kScale);
			 float int_bfromg_untag_JESup = h_bfromg_untag_JESup[is]->Integral(0,h_bfromg_untag_JESup[is]->GetXaxis()->GetNbins()+1);
			 h_b_untag_JESup[is] = (TH1F*)f->Get(hname_b_untag_JESup.c_str());
			 h_b_untag_JESup[is]->Scale(kScale);
			 float int_b_untag_JESup = h_b_untag_JESup[is]->Integral(0,h_b_untag_JESup[is]->GetXaxis()->GetNbins()+1);
			 h_cfromg_untag_JESup[is] = (TH1F*)f->Get(hname_cfromg_untag_JESup.c_str());
			 h_cfromg_untag_JESup[is]->Scale(kScale);
			 float int_cfromg_untag_JESup = h_cfromg_untag_JESup[is]->Integral(0,h_cfromg_untag_JESup[is]->GetXaxis()->GetNbins()+1);
			 h_c_untag_JESup[is] = (TH1F*)f->Get(hname_c_untag_JESup.c_str());
			 h_c_untag_JESup[is]->Scale(kScale);
			 float int_c_untag_JESup = h_c_untag_JESup[is]->Integral(0,h_c_untag_JESup[is]->GetXaxis()->GetNbins()+1);
			 h_l_untag_JESup[is] = (TH1F*)f->Get(hname_l_untag_JESup.c_str());
			 h_l_untag_JESup[is]->Scale(kScale);
			 float int_l_untag_JESup = h_l_untag_JESup[is]->Integral(0,h_l_untag_JESup[is]->GetXaxis()->GetNbins()+1);
		 
	//      float int_total_untag_JESup = int_bfromg_untag_JESup+int_b_untag_JESup+int_cfromg_untag_JESup+int_c_untag_JESup+int_l_untag_JESup;
	//      float k_lumi_untag_JESup = int_data_untag/int_total_untag_JESup;
	//	 float int_total_untag_JESup = int_total_SYSup.at(is);
	//	 float k_lumi_untag_JESup = k_lumi_SYSup.at(is);

			 float k_lumi_untag_JESup;
			 if(varName.at(ifile)=="JP") k_lumi_untag_JESup = k_lumi_SYSup.at(is);
			 else k_lumi_untag_JESup = k_lumi_SYSup_allpt.at(i).at(is); //overrides and take value of norm of preuntag JP 
		 
			 h_bfromg_untag_JESup[is]->Scale(k_lumi_untag_JESup);
			 if(i==8 && DEBUG_untag)std::cout << "after scale	:h_bfromg_untag_JESup["<< is <<"]("<< hname_bfromg_untag_JESup.c_str()<< ")->Integral() = "<< h_bfromg_untag_JESup[is]->Integral() << std::endl;
			 h_b_untag_JESup[is]->Scale(k_lumi_untag_JESup);
			 h_cfromg_untag_JESup[is]->Scale(k_lumi_untag_JESup);
			 h_c_untag_JESup[is]->Scale(k_lumi_untag_JESup);
			 h_l_untag_JESup[is]->Scale(k_lumi_untag_JESup);

	// 		k_norm_bfromg_untag = 1./h_bfromg_untag_JESup[is]->Integral();
	// 		if(i==8 && DEBUG_untag)std::cout << "k_norm_bfromg_JESup["<<is<<"]  = "<< k_norm_bfromg << std::endl;
	// 		k_norm_b_untag = 1./h_b_untag_JESup[is]->Integral();
	// 		k_norm_cfromg_untag = 1./h_cfromg_untag_JESup[is]->Integral();
	// 		k_norm_c_untag = 1./h_c_untag_JESup[is]->Integral();
	// 		k_norm_l_untag = 1./h_l_untag_JESup[is]->Integral();
	// 
	// 	     h_bfromg_untag_JESup[is]->Scale(k_norm_bfromg_untag);
	// 	     if(i==8 && DEBUG_untag)std::cout << "after scale	:h_bfromg_untag_JESup["<< is <<"]("<< hname_bfromg_untag_JESup.c_str()<< ")->Integral() = "<< h_bfromg_untag_JESup[is]->Integral() << std::endl;
	// 	     h_b_untag_JESup[is]->Scale(k_norm_b_untag);
	// 	     h_cfromg_untag_JESup[is]->Scale(k_norm_cfromg_untag);
	// 	     h_c_untag_JESup[is]->Scale(k_norm_c_untag);
	// 	     h_l_untag_JESup[is]->Scale(k_norm_l_untag);

			 // untag JESdown
			 std::string hname_bfromg_untag_JESdown = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_"+sysName[is]+"down_bfromg";
			 std::string hname_b_untag_JESdown = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_"+sysName[is]+"down_b";
			 std::string hname_cfromg_untag_JESdown = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_"+sysName[is]+"down_cfromg";
			 std::string hname_c_untag_JESdown = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_"+sysName[is]+"down_c";
			 std::string hname_l_untag_JESdown = "UNWEIGHTED__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_"+sysName[is]+"down_l";

			 if( sysName[is] == "NTRACKS" || sysName[is] == "CFRAG" || sysName[is] == "CD" || sysName[is] == "K0L" )
			   {
				  hname_bfromg_untag_JESdown = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_bfromg";
				  hname_b_untag_JESdown = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_b";
				  hname_cfromg_untag_JESdown = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_cfromg";
				  hname_c_untag_JESdown = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_c";
				  hname_l_untag_JESdown = sysName[is]+"__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_l";
			   }	     

			 if( sysName[is] == "BFRAG" || sysName[is] == "PU" )
			   {
				  hname_bfromg_untag_JESdown = sysName[is]+"DOWN__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_bfromg";
				  hname_b_untag_JESdown = sysName[is]+"DOWN__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_b";
				  hname_cfromg_untag_JESdown = sysName[is]+"DOWN__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_cfromg";
				  hname_c_untag_JESdown = sysName[is]+"DOWN__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_c";
				  hname_l_untag_JESdown = sysName[is]+"DOWN__"+QCDlabel+"__FatJet_"+var+"_"+tag+"fail_"+pt+"_l";
			   }	     
		 
			 h_bfromg_untag_JESdown[is] = (TH1F*)f->Get(hname_bfromg_untag_JESdown.c_str());
			 if(i==8 && DEBUG_untag)std::cout << "" << std::endl;
			 if(i==8 && DEBUG_untag)std::cout << "before		:h_bfromg_untag_JESdown["<< is <<"]("<< hname_bfromg_untag_JESdown.c_str()<< ")->Integral() = "<< h_bfromg_untag_JESdown[is]->Integral() << std::endl;
			 h_bfromg_untag_JESdown[is]->Scale(kScale);
			 float int_bfromg_untag_JESdown = h_bfromg_untag_JESdown[is]->Integral(0,h_bfromg_untag_JESdown[is]->GetXaxis()->GetNbins()+1);
			 h_b_untag_JESdown[is] = (TH1F*)f->Get(hname_b_untag_JESdown.c_str());
			 h_b_untag_JESdown[is]->Scale(kScale);
			 float int_b_untag_JESdown = h_b_untag_JESdown[is]->Integral(0,h_b_untag_JESdown[is]->GetXaxis()->GetNbins()+1);
			 h_cfromg_untag_JESdown[is] = (TH1F*)f->Get(hname_cfromg_untag_JESdown.c_str());
			 h_cfromg_untag_JESdown[is]->Scale(kScale);
			 float int_cfromg_untag_JESdown = h_cfromg_untag_JESdown[is]->Integral(0,h_cfromg_untag_JESdown[is]->GetXaxis()->GetNbins()+1);
			 h_c_untag_JESdown[is] = (TH1F*)f->Get(hname_c_untag_JESdown.c_str());
			 h_c_untag_JESdown[is]->Scale(kScale);
			 float int_c_untag_JESdown = h_c_untag_JESdown[is]->Integral(0,h_c_untag_JESdown[is]->GetXaxis()->GetNbins()+1);
			 h_l_untag_JESdown[is] = (TH1F*)f->Get(hname_l_untag_JESdown.c_str());
			 h_l_untag_JESdown[is]->Scale(kScale);
			 float int_l_untag_JESdown = h_l_untag_JESdown[is]->Integral(0,h_l_untag_JESdown[is]->GetXaxis()->GetNbins()+1);

	//      float int_total_untag_JESdown = int_bfromg_untag_JESdown+int_b_untag_JESdown+int_cfromg_untag_JESdown+int_c_untag_JESdown+int_l_untag_JESdown;
	//      float k_lumi_untag_JESdown = int_data_untag/int_total_untag_JESdown;
	//	 float int_total_untag_JESdown = int_total_SYSdown.at(is);
	//	 float k_lumi_untag_JESdown = k_lumi_SYSdown.at(is);

			 float k_lumi_untag_JESdown;
			 if(varName.at(ifile)=="JP") k_lumi_untag_JESdown = k_lumi_SYSdown.at(is);
			 else k_lumi_untag_JESdown = k_lumi_SYSdown_allpt.at(i).at(is); //overrides and take value of norm of preuntag JP 
		 
			 if( sysName[is] == "BFRAG" || sysName[is] == "PU" || sysName[is] == "JES" ){ //because only these sys have "down"s other sys are copies of their "up"s.
				 h_bfromg_untag_JESdown[is]->Scale(k_lumi_untag_JESdown);
				 if(i==8 && DEBUG_untag)std::cout << "after scale	:h_bfromg_untag_JESdown["<< is <<"]("<< hname_bfromg_untag_JESdown.c_str()<< ")->Integral() = "<< h_bfromg_untag_JESdown[is]->Integral() << std::endl;
				 h_b_untag_JESdown[is]->Scale(k_lumi_untag_JESdown);
				 h_cfromg_untag_JESdown[is]->Scale(k_lumi_untag_JESdown);
				 h_c_untag_JESdown[is]->Scale(k_lumi_untag_JESdown);
				 h_l_untag_JESdown[is]->Scale(k_lumi_untag_JESdown);
			 } 

	// 		k_norm_bfromg_untag = 1./h_bfromg_untag_JESdown[is]->Integral();
	// 		if(i==8 && DEBUG_untag)std::cout << "k_norm_bfromg_untag_JESdown["<<is<<"]  = "<< k_norm_bfromg_untag << std::endl;
	// 		k_norm_b_untag = 1./h_b_untag_JESdown[is]->Integral();
	// 		k_norm_cfromg_untag = 1./h_cfromg_untag_JESdown[is]->Integral();
	// 		k_norm_c_untag = 1./h_c_untag_JESdown[is]->Integral();
	// 		k_norm_l_untag = 1./h_l_untag_JESdown[is]->Integral();
	// 
	// 	     h_bfromg_untag_JESdown[is]->Scale(k_norm_bfromg_untag);
	// 	     if(i==8 && DEBUG_untag)std::cout << "after scale	:h_bfromg_untag_JESdown["<< is <<"]("<< hname_bfromg_untag_JESdown.c_str()<< ")->Integral() = "<< h_bfromg_untag_JESdown[is]->Integral() << std::endl;
	// 	     h_b_untag_JESdown[is]->Scale(k_norm_b_untag);
	// 	     h_cfromg_untag_JESdown[is]->Scale(k_norm_cfromg_untag);
	// 	     h_c_untag_JESdown[is]->Scale(k_norm_c_untag);
	// 	     h_l_untag_JESdown[is]->Scale(k_norm_l_untag);
		  }	

		if(DEBUG)std::cout<< "CHECK F "<< std::endl;

		// COMBINATIONS
		if(i==8 && DEBUG_rebin)std::cout << "" << std::endl;
		if(i==8 && DEBUG_rebin)std::cout << "----------------------------------------" << std::endl;
		if(i==8 && DEBUG_rebin)std::cout << "Rebin -  PRETAG - "  << pt << std::endl;
		if(i==8 && DEBUG_rebin)std::cout << "----------------------------------------" << std::endl;
		if(i==8 && DEBUG_rebin)std::cout << "nReBin = " << nReBin <<std::endl;
		if(i==8 && DEBUG_rebin)std::cout << "" << std::endl;
	
		std::string hname_data_opt = hname_data+"_opt";
		TH1F *h_data_opt = (TH1F*)h_data->Rebin(nReBin,hname_data_opt.c_str());
		std::string hname_data_tag_opt = hname_data_tag+"_opt";
		TH1F *h_data_tag_opt = (TH1F*)h_data_tag->Rebin(nReBin,hname_data_tag_opt.c_str());
		std::string hname_data_untag_opt = hname_data_untag+"_opt";
		TH1F *h_data_untag_opt = (TH1F*)h_data_untag->Rebin(nReBin,hname_data_untag_opt.c_str());
	
		std::string hname_bfromg_opt = hname_bfromg+"_opt";
		TH1F *h_bfromg_opt = (TH1F*)h_bfromg->Rebin(nReBin,hname_bfromg_opt.c_str());
		if(i==8 && DEBUG_rebin)std::cout << "Rebin :h_bfromg_opt("<< hname_bfromg_opt.c_str()<< "->Integral() = "<< h_bfromg_opt->Integral() << std::endl;
		std::string hname_b_opt = hname_b+"_opt";
		TH1F *h_b_opt = (TH1F*)h_b->Rebin(nReBin,hname_b_opt.c_str());
		std::string hname_cfromg_opt = hname_cfromg+"_opt";
		TH1F *h_cfromg_opt = (TH1F*)h_cfromg->Rebin(nReBin,hname_cfromg_opt.c_str());
		std::string hname_c_opt = hname_c+"_opt";
		TH1F *h_c_opt = (TH1F*)h_c->Rebin(nReBin,hname_c_opt.c_str());
		std::string hname_l_opt = hname_l+"_opt";
		TH1F *h_l_opt = (TH1F*)h_l->Rebin(nReBin,hname_l_opt.c_str());
		if(DEBUG)std::cout<< "CHECK G "<< std::endl;

		// combine pre-tag
		std::string hname_b_cfromg_opt = hname_b+"_cfromg_opt";
		TH1F *h_b_cfromg_opt = (TH1F*)h_b_opt->Clone(hname_b_cfromg_opt.c_str());
		h_b_cfromg_opt->Add(h_cfromg_opt);
		h_b_cfromg_opt->Scale(kScale_b_cfromg);
		std::string hname_c_l_opt = hname_c+"_l_opt";
		TH1F *h_c_l_opt = (TH1F*)h_c_opt->Clone(hname_c_l_opt.c_str());
		h_c_l_opt->Add(h_l_opt);
		h_c_l_opt->Scale(kScale_c_l);
		std::string hname_b_cfromg_c_l_opt = hname_b+"_cfromg_c_l_opt";
		TH1F *h_b_cfromg_c_l_opt = (TH1F*)h_b_cfromg_opt->Clone(hname_b_cfromg_c_l_opt.c_str());
		h_b_cfromg_c_l_opt->Add(h_c_l_opt);

		TH1F *h_bfromg_JESup_opt[100];
		TH1F *h_b_JESup_opt[100];
		TH1F *h_cfromg_JESup_opt[100];
		TH1F *h_c_JESup_opt[100];
		TH1F *h_l_JESup_opt[100];

		TH1F *h_bfromg_JESdown_opt[100];
		TH1F *h_b_JESdown_opt[100];
		TH1F *h_cfromg_JESdown_opt[100];
		TH1F *h_c_JESdown_opt[100];
		TH1F *h_l_JESdown_opt[100];
	
		TH1F *h_b_cfromg_JESup_opt[100];
		TH1F *h_c_l_JESup_opt[100];
		TH1F *h_b_cfromg_c_l_JESup_opt[100];

		TH1F *h_b_cfromg_JESdown_opt[100];
		TH1F *h_c_l_JESdown_opt[100];
		TH1F *h_b_cfromg_c_l_JESdown_opt[100];
		if(DEBUG)std::cout<< "CHECK H "<< std::endl;

		for(int is=0;is<nSys;is++)
		  {	    		
			 std::string hname_bfromg_JESup_opt = hname_bfromg+"_opt_"+sysName[is]+"up";
			 h_bfromg_JESup_opt[is] = (TH1F*)h_bfromg_JESup[is]->Rebin(nReBin,hname_bfromg_JESup_opt.c_str());
			 if(i==8 && DEBUG_rebin)std::cout << "Rebin :h_bfromg_JESup_opt["<<is<<"]("<< hname_bfromg_JESup_opt.c_str()<< "->Integral() = "<< h_bfromg_JESup_opt[is]->Integral() << std::endl;
			 std::string hname_b_JESup_opt = hname_b+"_opt_"+sysName[is]+"up";
			 h_b_JESup_opt[is] = (TH1F*)h_b_JESup[is]->Rebin(nReBin,hname_b_JESup_opt.c_str());
			 std::string hname_cfromg_JESup_opt = hname_cfromg+"_opt_"+sysName[is]+"up";
			 h_cfromg_JESup_opt[is] = (TH1F*)h_cfromg_JESup[is]->Rebin(nReBin,hname_cfromg_JESup_opt.c_str());
			 std::string hname_c_JESup_opt = hname_c+"_opt_"+sysName[is]+"up";
			 h_c_JESup_opt[is] = (TH1F*)h_c_JESup[is]->Rebin(nReBin,hname_c_JESup_opt.c_str());
			 std::string hname_l_JESup_opt = hname_l+"_opt_"+sysName[is]+"up";
			 h_l_JESup_opt[is] = (TH1F*)h_l_JESup[is]->Rebin(nReBin,hname_l_JESup_opt.c_str());
		 
			 std::string hname_bfromg_JESdown_opt = hname_bfromg+"_opt_"+sysName[is]+"down";
			 h_bfromg_JESdown_opt[is] = (TH1F*)h_bfromg_JESdown[is]->Rebin(nReBin,hname_bfromg_JESdown_opt.c_str());
			 if(i==8 && DEBUG_rebin)std::cout << "Rebin :h_bfromg_JESdown_opt["<<is<<"]("<< hname_bfromg_JESdown_opt.c_str()<< "->Integral() = "<< h_bfromg_JESdown_opt[is]->Integral() << std::endl;
			 std::string hname_b_JESdown_opt = hname_b+"_opt_"+sysName[is]+"down";
			 h_b_JESdown_opt[is] = (TH1F*)h_b_JESdown[is]->Rebin(nReBin,hname_b_JESdown_opt.c_str());
			 std::string hname_cfromg_JESdown_opt = hname_cfromg+"_opt_"+sysName[is]+"down";
			 h_cfromg_JESdown_opt[is] = (TH1F*)h_cfromg_JESdown[is]->Rebin(nReBin,hname_cfromg_JESdown_opt.c_str());
			 std::string hname_c_JESdown_opt = hname_c+"_opt_"+sysName[is]+"down";
			 h_c_JESdown_opt[is] = (TH1F*)h_c_JESdown[is]->Rebin(nReBin,hname_c_JESdown_opt.c_str());
			 std::string hname_l_JESdown_opt = hname_l+"_opt_"+sysName[is]+"down";
			 h_l_JESdown_opt[is] = (TH1F*)h_l_JESdown[is]->Rebin(nReBin,hname_l_JESdown_opt.c_str());

			 std::string hname_b_cfromg_JESup_opt = hname_b+"_cfromg_opt_"+sysName[is]+"up";
			 h_b_cfromg_JESup_opt[is] = (TH1F*)h_b_JESup_opt[is]->Clone(hname_b_cfromg_JESup_opt.c_str());
			 h_b_cfromg_JESup_opt[is]->Add(h_cfromg_JESup_opt[is]);
			 h_b_cfromg_JESup_opt[is]->Scale(kScale_b_cfromg);
			 std::string hname_c_l_JESup_opt = hname_c+"_l_opt_"+sysName[is]+"up";
			 h_c_l_JESup_opt[is] = (TH1F*)h_c_JESup_opt[is]->Clone(hname_c_l_JESup_opt.c_str());
			 h_c_l_JESup_opt[is]->Add(h_l_JESup_opt[is]);
			 h_c_l_JESup_opt[is]->Scale(kScale_c_l);
			 std::string hname_b_cfromg_c_l_JESup_opt = hname_b+"_cfromg_c_l_opt_"+sysName[is]+"up";
			 h_b_cfromg_c_l_JESup_opt[is] = (TH1F*)h_b_cfromg_JESup_opt[is]->Clone(hname_b_cfromg_c_l_JESup_opt.c_str());
			 h_b_cfromg_c_l_JESup_opt[is]->Add(h_c_l_JESup_opt[is]);
		 
			 std::string hname_b_cfromg_JESdown_opt = hname_b+"_cfromg_opt_"+sysName[is]+"down";
			 h_b_cfromg_JESdown_opt[is] = (TH1F*)h_b_JESdown_opt[is]->Clone(hname_b_cfromg_JESdown_opt.c_str());
			 h_b_cfromg_JESdown_opt[is]->Add(h_cfromg_JESdown_opt[is]);
			 h_b_cfromg_JESdown_opt[is]->Scale(kScale_b_cfromg);
			 std::string hname_c_l_JESdown_opt = hname_c+"_l_opt_"+sysName[is]+"down";
			 h_c_l_JESdown_opt[is] = (TH1F*)h_c_JESdown_opt[is]->Clone(hname_c_l_JESdown_opt.c_str());
			 h_c_l_JESdown_opt[is]->Add(h_l_JESdown_opt[is]);
			 h_c_l_JESdown_opt[is]->Scale(kScale_c_l);
			 std::string hname_b_cfromg_c_l_JESdown_opt = hname_b+"_cfromg_c_l_opt_"+sysName[is]+"down";
			 h_b_cfromg_c_l_JESdown_opt[is] = (TH1F*)h_b_cfromg_JESdown_opt[is]->Clone(hname_b_cfromg_c_l_JESdown_opt.c_str());
			 h_b_cfromg_c_l_JESdown_opt[is]->Add(h_c_l_JESdown_opt[is]);
		  }	

		// TAG
		if(DEBUG)std::cout<< "CHECK I "<< std::endl;
		if(i==8 && DEBUG_rebin)std::cout << "" << std::endl;
		if(i==8 && DEBUG_rebin)std::cout << "----------------------------------------" << std::endl;
		if(i==8 && DEBUG_rebin)std::cout << "Rebin -  TAG - "  << pt << std::endl;
		if(i==8 && DEBUG_rebin)std::cout << "----------------------------------------" << std::endl;
		if(i==8 && DEBUG_rebin)std::cout << "nReBin = " << nReBin <<std::endl;
		if(i==8 && DEBUG_rebin)std::cout << "" << std::endl;

		std::string hname_bfromg_tag_opt = hname_bfromg_tag+"_opt";
		TH1F *h_bfromg_tag_opt = (TH1F*)h_bfromg_tag->Rebin(nReBin,hname_bfromg_tag_opt.c_str());
		if(i==8 && DEBUG_rebin)std::cout << "Rebin :h_bfromg_tag_opt("<< hname_bfromg_tag_opt.c_str()<< "->Integral() = "<< h_bfromg_tag_opt->Integral() << std::endl;
		std::string hname_b_tag_opt = hname_b_tag+"_opt";
		TH1F *h_b_tag_opt = (TH1F*)h_b_tag->Rebin(nReBin,hname_b_tag_opt.c_str());
		std::string hname_cfromg_tag_opt = hname_cfromg_tag+"_opt";
		TH1F *h_cfromg_tag_opt = (TH1F*)h_cfromg_tag->Rebin(nReBin,hname_cfromg_tag_opt.c_str());
		std::string hname_c_tag_opt = hname_c_tag+"_opt";
		TH1F *h_c_tag_opt = (TH1F*)h_c_tag->Rebin(nReBin,hname_c_tag_opt.c_str());
		std::string hname_l_tag_opt = hname_l_tag+"_opt";
		TH1F *h_l_tag_opt = (TH1F*)h_l_tag->Rebin(nReBin,hname_l_tag_opt.c_str());
	
		// combine tag
		std::string hname_b_cfromg_tag_opt = hname_b_tag+"_cfromg_opt";
		TH1F *h_b_cfromg_tag_opt = (TH1F*)h_b_tag_opt->Clone(hname_b_cfromg_tag_opt.c_str());
		h_b_cfromg_tag_opt->Add(h_cfromg_tag_opt);
		h_b_cfromg_tag_opt->Scale(kScale_b_cfromg);
		std::string hname_c_l_tag_opt = hname_c_tag+"_l_opt";
		TH1F *h_c_l_tag_opt = (TH1F*)h_c_tag_opt->Clone(hname_c_l_tag_opt.c_str());
		h_c_l_tag_opt->Add(h_l_tag_opt);
		h_c_l_tag_opt->Scale(kScale_c_l);
		std::string hname_b_cfromg_c_l_tag_opt = hname_b_tag+"_cfromg_c_l_opt";
		TH1F *h_b_cfromg_c_l_tag_opt = (TH1F*)h_b_cfromg_tag_opt->Clone(hname_b_cfromg_c_l_tag_opt.c_str());
		h_b_cfromg_c_l_tag_opt->Add(h_c_l_tag_opt);

		TH1F *h_bfromg_tag_JESup_opt[100];
		TH1F *h_b_tag_JESup_opt[100];
		TH1F *h_cfromg_tag_JESup_opt[100];
		TH1F *h_c_tag_JESup_opt[100];
		TH1F *h_l_tag_JESup_opt[100];

		TH1F *h_bfromg_tag_JESdown_opt[100];
		TH1F *h_b_tag_JESdown_opt[100];
		TH1F *h_cfromg_tag_JESdown_opt[100];
		TH1F *h_c_tag_JESdown_opt[100];
		TH1F *h_l_tag_JESdown_opt[100];
	
		TH1F *h_b_cfromg_tag_JESup_opt[100];
		TH1F *h_c_l_tag_JESup_opt[100];
		TH1F *h_b_cfromg_c_l_tag_JESup_opt[100];

		TH1F *h_b_cfromg_tag_JESdown_opt[100];
		TH1F *h_c_l_tag_JESdown_opt[100];
		TH1F *h_b_cfromg_c_l_tag_JESdown_opt[100];
		if(DEBUG)std::cout<< "CHECK J "<< std::endl;
	
		for(int is=0;is<nSys;is++)
		  {
			 std::string hname_bfromg_tag_JESup_opt = hname_bfromg_tag+"_opt_"+sysName[is]+"up";
			 h_bfromg_tag_JESup_opt[is] = (TH1F*)h_bfromg_tag_JESup[is]->Rebin(nReBin,hname_bfromg_tag_JESup_opt.c_str());
			 if(i==8 && DEBUG_rebin)std::cout << "Rebin :h_bfromg_tag_JESup_opt["<<is<<"]("<< hname_bfromg_tag_JESup_opt.c_str()<< "->Integral() = "<< h_bfromg_tag_JESup_opt[is]->Integral() << std::endl;
			 std::string hname_b_tag_JESup_opt = hname_b_tag+"_opt_"+sysName[is]+"up";
			 h_b_tag_JESup_opt[is] = (TH1F*)h_b_tag_JESup[is]->Rebin(nReBin,hname_b_tag_JESup_opt.c_str());
			 std::string hname_cfromg_tag_JESup_opt = hname_cfromg_tag+"_opt_"+sysName[is]+"up";
			 h_cfromg_tag_JESup_opt[is] = (TH1F*)h_cfromg_tag_JESup[is]->Rebin(nReBin,hname_cfromg_tag_JESup_opt.c_str());
			 std::string hname_c_tag_JESup_opt = hname_c_tag+"_opt_"+sysName[is]+"up";
			 h_c_tag_JESup_opt[is] = (TH1F*)h_c_tag_JESup[is]->Rebin(nReBin,hname_c_tag_JESup_opt.c_str());
			 std::string hname_l_tag_JESup_opt = hname_l_tag+"_opt_"+sysName[is]+"up";
			 h_l_tag_JESup_opt[is] = (TH1F*)h_l_tag_JESup[is]->Rebin(nReBin,hname_l_tag_JESup_opt.c_str());

			 std::string hname_bfromg_tag_JESdown_opt = hname_bfromg_tag+"_opt_"+sysName[is]+"down";
			 h_bfromg_tag_JESdown_opt[is] = (TH1F*)h_bfromg_tag_JESdown[is]->Rebin(nReBin,hname_bfromg_tag_JESdown_opt.c_str());
			 if(i==8 && DEBUG_rebin)std::cout << "Rebin :h_bfromg_tag_JESdown_opt["<<is<<"]("<< hname_bfromg_tag_JESdown_opt.c_str()<< "->Integral() = "<< h_bfromg_tag_JESdown_opt[is]->Integral() << std::endl;
			 std::string hname_b_tag_JESdown_opt = hname_b_tag+"_opt_"+sysName[is]+"down";
			 h_b_tag_JESdown_opt[is] = (TH1F*)h_b_tag_JESdown[is]->Rebin(nReBin,hname_b_tag_JESdown_opt.c_str());
			 std::string hname_cfromg_tag_JESdown_opt = hname_cfromg_tag+"_opt_"+sysName[is]+"down";
			 h_cfromg_tag_JESdown_opt[is] = (TH1F*)h_cfromg_tag_JESdown[is]->Rebin(nReBin,hname_cfromg_tag_JESdown_opt.c_str());
			 std::string hname_c_tag_JESdown_opt = hname_c_tag+"_opt_"+sysName[is]+"down";
			 h_c_tag_JESdown_opt[is] = (TH1F*)h_c_tag_JESdown[is]->Rebin(nReBin,hname_c_tag_JESdown_opt.c_str());
			 std::string hname_l_tag_JESdown_opt = hname_l_tag+"_opt_"+sysName[is]+"down";
			 h_l_tag_JESdown_opt[is] = (TH1F*)h_l_tag_JESdown[is]->Rebin(nReBin,hname_l_tag_JESdown_opt.c_str());
	
			 std::string hname_b_cfromg_tag_JESup_opt = hname_b_tag+"_cfromg_opt_"+sysName[is]+"up";
			 h_b_cfromg_tag_JESup_opt[is] = (TH1F*)h_b_tag_JESup_opt[is]->Clone(hname_b_cfromg_tag_JESup_opt.c_str());
			 h_b_cfromg_tag_JESup_opt[is]->Add(h_cfromg_tag_JESup_opt[is]);
			 h_b_cfromg_tag_JESup_opt[is]->Scale(kScale_b_cfromg);
			 std::string hname_c_l_tag_JESup_opt = hname_c_tag+"_l_opt_"+sysName[is]+"up";
			 h_c_l_tag_JESup_opt[is] = (TH1F*)h_c_tag_JESup_opt[is]->Clone(hname_c_l_tag_JESup_opt.c_str());
			 h_c_l_tag_JESup_opt[is]->Add(h_l_tag_JESup_opt[is]);
			 h_c_l_tag_JESup_opt[is]->Scale(kScale_c_l);
			 std::string hname_b_cfromg_c_l_tag_JESup_opt = hname_b_tag+"_cfromg_c_l_opt_"+sysName[is]+"up";
			 h_b_cfromg_c_l_tag_JESup_opt[is] = (TH1F*)h_b_cfromg_tag_JESup_opt[is]->Clone(hname_b_cfromg_c_l_tag_JESup_opt.c_str());
			 h_b_cfromg_c_l_tag_JESup_opt[is]->Add(h_c_l_tag_JESup_opt[is]);
		 
			 std::string hname_b_cfromg_tag_JESdown_opt = hname_b_tag+"_cfromg_opt_"+sysName[is]+"down";
			 h_b_cfromg_tag_JESdown_opt[is] = (TH1F*)h_b_tag_JESdown_opt[is]->Clone(hname_b_cfromg_tag_JESdown_opt.c_str());
			 h_b_cfromg_tag_JESdown_opt[is]->Add(h_cfromg_tag_JESdown_opt[is]);
			 h_b_cfromg_tag_JESdown_opt[is]->Scale(kScale_b_cfromg);
			 std::string hname_c_l_tag_JESdown_opt = hname_c_tag+"_l_opt_"+sysName[is]+"down";
			 h_c_l_tag_JESdown_opt[is] = (TH1F*)h_c_tag_JESdown_opt[is]->Clone(hname_c_l_tag_JESdown_opt.c_str());
			 h_c_l_tag_JESdown_opt[is]->Add(h_l_tag_JESdown_opt[is]);
			 h_c_l_tag_JESdown_opt[is]->Scale(kScale_c_l);
			 std::string hname_b_cfromg_c_l_tag_JESdown_opt = hname_b_tag+"_cfromg_c_l_opt_"+sysName[is]+"down";
			 h_b_cfromg_c_l_tag_JESdown_opt[is] = (TH1F*)h_b_cfromg_tag_JESdown_opt[is]->Clone(hname_b_cfromg_c_l_tag_JESdown_opt.c_str());
			 h_b_cfromg_c_l_tag_JESdown_opt[is]->Add(h_c_l_tag_JESdown_opt[is]);
		  }	

		if(DEBUG)std::cout<< "CHECK K "<< std::endl;
		if(i==8 && DEBUG_rebin)std::cout << "" << std::endl;
		if(i==8 && DEBUG_rebin)std::cout << "----------------------------------------" << std::endl;
		if(i==8 && DEBUG_rebin)std::cout << "Rebin -  UNTAG - "  << pt << std::endl;
		if(i==8 && DEBUG_rebin)std::cout << "----------------------------------------" << std::endl;
		if(i==8 && DEBUG_rebin)std::cout << "nReBin = " << nReBin <<std::endl;
		if(i==8 && DEBUG_rebin)std::cout << "" << std::endl;

	
		std::string hname_bfromg_untag_opt = hname_bfromg_untag+"_opt";
		TH1F *h_bfromg_untag_opt = (TH1F*)h_bfromg_untag->Rebin(nReBin,hname_bfromg_untag_opt.c_str());
		if(i==8 && DEBUG_rebin)std::cout << "Rebin :h_bfromg_untag_opt("<< hname_bfromg_untag_opt.c_str()<< "->Integral() = "<< h_bfromg_untag_opt->Integral() << std::endl;
		std::string hname_b_untag_opt = hname_b_untag+"_opt";
		TH1F *h_b_untag_opt = (TH1F*)h_b_untag->Rebin(nReBin,hname_b_untag_opt.c_str());
		std::string hname_cfromg_untag_opt = hname_cfromg_untag+"_opt";
		TH1F *h_cfromg_untag_opt = (TH1F*)h_cfromg_untag->Rebin(nReBin,hname_cfromg_untag_opt.c_str());
		std::string hname_c_untag_opt = hname_c_untag+"_opt";
		TH1F *h_c_untag_opt = (TH1F*)h_c_untag->Rebin(nReBin,hname_c_untag_opt.c_str());
		std::string hname_l_untag_opt = hname_l_untag+"_opt";
		TH1F *h_l_untag_opt = (TH1F*)h_l_untag->Rebin(nReBin,hname_l_untag_opt.c_str());
	
		// combine untag
		std::string hname_b_cfromg_untag_opt = hname_b_untag+"_cfromg_opt";
		TH1F *h_b_cfromg_untag_opt = (TH1F*)h_b_untag_opt->Clone(hname_b_cfromg_untag_opt.c_str());
		h_b_cfromg_untag_opt->Add(h_cfromg_untag_opt);
		h_b_cfromg_untag_opt->Scale(kScale_b_cfromg);
		std::string hname_c_l_untag_opt = hname_c_untag+"_l_opt";
		TH1F *h_c_l_untag_opt = (TH1F*)h_c_untag_opt->Clone(hname_c_l_untag_opt.c_str());
		h_c_l_untag_opt->Add(h_l_untag_opt);
		h_c_l_untag_opt->Scale(kScale_c_l);
		std::string hname_b_cfromg_c_l_untag_opt = hname_b_untag+"_cfromg_c_l_opt";
		TH1F *h_b_cfromg_c_l_untag_opt = (TH1F*)h_b_cfromg_untag_opt->Clone(hname_b_cfromg_c_l_untag_opt.c_str());
		h_b_cfromg_c_l_untag_opt->Add(h_c_l_untag_opt);

		TH1F *h_bfromg_untag_JESup_opt[100];
		TH1F *h_b_untag_JESup_opt[100];
		TH1F *h_cfromg_untag_JESup_opt[100];
		TH1F *h_c_untag_JESup_opt[100];
		TH1F *h_l_untag_JESup_opt[100];

		TH1F *h_bfromg_untag_JESdown_opt[100];
		TH1F *h_b_untag_JESdown_opt[100];
		TH1F *h_cfromg_untag_JESdown_opt[100];
		TH1F *h_c_untag_JESdown_opt[100];
		TH1F *h_l_untag_JESdown_opt[100];
	
		TH1F *h_b_cfromg_untag_JESup_opt[100];
		TH1F *h_c_l_untag_JESup_opt[100];
		TH1F *h_b_cfromg_c_l_untag_JESup_opt[100];

		TH1F *h_b_cfromg_untag_JESdown_opt[100];
		TH1F *h_c_l_untag_JESdown_opt[100];
		TH1F *h_b_cfromg_c_l_untag_JESdown_opt[100];
		if(DEBUG)std::cout<< "CHECK L "<< std::endl;
	
		for(int is=0;is<nSys;is++)
		  {	    					
			 std::string hname_bfromg_untag_JESup_opt = hname_bfromg_untag+"_opt_"+sysName[is]+"up";
			 h_bfromg_untag_JESup_opt[is] = (TH1F*)h_bfromg_untag_JESup[is]->Rebin(nReBin,hname_bfromg_untag_JESup_opt.c_str());
			 if(i==8 && DEBUG_rebin)std::cout << "Rebin :h_bfromg_untag_JESup_opt["<<is<<"]("<< hname_bfromg_untag_JESup_opt.c_str()<< "->Integral() = "<< h_bfromg_untag_JESup_opt[is]->Integral() << std::endl;
			 std::string hname_b_untag_JESup_opt = hname_b_untag+"_opt_"+sysName[is]+"up";
			 h_b_untag_JESup_opt[is] = (TH1F*)h_b_untag_JESup[is]->Rebin(nReBin,hname_b_untag_JESup_opt.c_str());
			 std::string hname_cfromg_untag_JESup_opt = hname_cfromg_untag+"_opt_"+sysName[is]+"up";
			 h_cfromg_untag_JESup_opt[is] = (TH1F*)h_cfromg_untag_JESup[is]->Rebin(nReBin,hname_cfromg_untag_JESup_opt.c_str());
			 std::string hname_c_untag_JESup_opt = hname_c_untag+"_opt_"+sysName[is]+"up";
			 h_c_untag_JESup_opt[is] = (TH1F*)h_c_untag_JESup[is]->Rebin(nReBin,hname_c_untag_JESup_opt.c_str());
			 std::string hname_l_untag_JESup_opt = hname_l_untag+"_opt_"+sysName[is]+"up";
			 h_l_untag_JESup_opt[is] = (TH1F*)h_l_untag_JESup[is]->Rebin(nReBin,hname_l_untag_JESup_opt.c_str());
		 
			 std::string hname_bfromg_untag_JESdown_opt = hname_bfromg_untag+"_opt_"+sysName[is]+"down";
			 h_bfromg_untag_JESdown_opt[is] = (TH1F*)h_bfromg_untag_JESdown[is]->Rebin(nReBin,hname_bfromg_untag_JESdown_opt.c_str());
			 if(i==8 && DEBUG_rebin)std::cout << "Rebin :h_bfromg_untag_JESdown_opt["<<is<<"]("<< hname_bfromg_untag_JESdown_opt.c_str()<< "->Integral() = "<< h_bfromg_untag_JESdown_opt[is]->Integral() << std::endl;
			 std::string hname_b_untag_JESdown_opt = hname_b_untag+"_opt_"+sysName[is]+"down";
			 h_b_untag_JESdown_opt[is] = (TH1F*)h_b_untag_JESdown[is]->Rebin(nReBin,hname_b_untag_JESdown_opt.c_str());
			 std::string hname_cfromg_untag_JESdown_opt = hname_cfromg_untag+"_opt_"+sysName[is]+"down";
			 h_cfromg_untag_JESdown_opt[is] = (TH1F*)h_cfromg_untag_JESdown[is]->Rebin(nReBin,hname_cfromg_untag_JESdown_opt.c_str());
			 std::string hname_c_untag_JESdown_opt = hname_c_untag+"_opt_"+sysName[is]+"down";
			 h_c_untag_JESdown_opt[is] = (TH1F*)h_c_untag_JESdown[is]->Rebin(nReBin,hname_c_untag_JESdown_opt.c_str());
			 std::string hname_l_untag_JESdown_opt = hname_l_untag+"_opt_"+sysName[is]+"down";
			 h_l_untag_JESdown_opt[is] = (TH1F*)h_l_untag_JESdown[is]->Rebin(nReBin,hname_l_untag_JESdown_opt.c_str());
		 
			 std::string hname_b_cfromg_untag_JESup_opt = hname_b_untag+"_cfromg_opt_"+sysName[is]+"up";
			 h_b_cfromg_untag_JESup_opt[is] = (TH1F*)h_b_untag_JESup_opt[is]->Clone(hname_b_cfromg_untag_JESup_opt.c_str());
			 h_b_cfromg_untag_JESup_opt[is]->Add(h_cfromg_untag_JESup_opt[is]);
			 h_b_cfromg_untag_JESup_opt[is]->Scale(kScale_b_cfromg);
			 std::string hname_c_l_untag_JESup_opt = hname_c_untag+"_l_opt_"+sysName[is]+"up";
			 h_c_l_untag_JESup_opt[is] = (TH1F*)h_c_untag_JESup_opt[is]->Clone(hname_c_l_untag_JESup_opt.c_str());
			 h_c_l_untag_JESup_opt[is]->Add(h_l_untag_JESup_opt[is]);
			 h_c_l_untag_JESup_opt[is]->Scale(kScale_c_l);
			 std::string hname_b_cfromg_c_l_untag_JESup_opt = hname_b_untag+"_cfromg_c_l_opt_"+sysName[is]+"up";
			 h_b_cfromg_c_l_untag_JESup_opt[is] = (TH1F*)h_b_cfromg_untag_JESup_opt[is]->Clone(hname_b_cfromg_c_l_untag_JESup_opt.c_str());
			 h_b_cfromg_c_l_untag_JESup_opt[is]->Add(h_c_l_untag_JESup_opt[is]);
		 
			 std::string hname_b_cfromg_untag_JESdown_opt = hname_b_untag+"_cfromg_opt_"+sysName[is]+"down";
			 h_b_cfromg_untag_JESdown_opt[is] = (TH1F*)h_b_untag_JESdown_opt[is]->Clone(hname_b_cfromg_untag_JESdown_opt.c_str());
			 h_b_cfromg_untag_JESdown_opt[is]->Add(h_cfromg_untag_JESdown_opt[is]);
			 h_b_cfromg_untag_JESdown_opt[is]->Scale(kScale_b_cfromg);
			 std::string hname_c_l_untag_JESdown_opt = hname_c_untag+"_l_opt_"+sysName[is]+"down";
			 h_c_l_untag_JESdown_opt[is] = (TH1F*)h_c_untag_JESdown_opt[is]->Clone(hname_c_l_untag_JESdown_opt.c_str());
			 h_c_l_untag_JESdown_opt[is]->Add(h_l_untag_JESdown_opt[is]);
			 h_c_l_untag_JESdown_opt[is]->Scale(kScale_c_l);
			 std::string hname_b_cfromg_c_l_untag_JESdown_opt = hname_b_untag+"_cfromg_c_l_opt_"+sysName[is]+"down";
			 h_b_cfromg_c_l_untag_JESdown_opt[is] = (TH1F*)h_b_cfromg_untag_JESdown_opt[is]->Clone(hname_b_cfromg_c_l_untag_JESdown_opt.c_str());
			 h_b_cfromg_c_l_untag_JESdown_opt[is]->Add(h_c_l_untag_JESdown_opt[is]);
		  }	
		 }
		if(DEBUG)if(DEBUG)std::cout<< "CHECK M "<< std::endl;
		
		
   
	   fNew->Write();
	   fNew->Close();
	   
	   f->Close();
   }
   
}

void convert(){
// 	TString date = "Apr24-2018_testing_Mar15file/"; //fixed normalization v2, post,untag take norm from pre
// 	TString date = "Apr24-2018_testing_Apr3file/"; //fixed normalization v2, post,untag take norm from pre
// 	TString date = "Apr27-2018_reprocess_Mar15file_Apr3file_combined/"; //fixed normalization v3, post,untag of JPhas,JPnoSV,SVmass take norm from pre of JP.
	TString date = "Apr30-2018_reprocess_Mar15file_Apr3file_combined_v2/"; //fixed normalization v4, post,untag of JPhas,JPnoSV,SVmass take norm from pre of JP.
	system("mkdir -vp SFtemplates/"+date);

// 	TString fin;
// 	std::string fout;
// 	std::string kScaleStr;

	std::vector<TString> fin;
	std::vector<TString> fout;
	std::vector<std::string> varName;
	std::string kScaleStr;

//Apr3-2018 _dataUseMCJPcalib =============================
/*
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_JPhasSV_ptReweighted_dataUseMCJPcalib_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_JPhasSV_ptReweighted_dataUseMCJPcalib_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 	
// 	}
// 
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_JPhasSV_ptReweighted_dataUseMCJPcalib_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_JPhasSV_ptReweighted_dataUseMCJPcalib_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 	
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 		
// 	}

// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_JPnoSV_ptReweighted_dataUseMCJPcalib_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_JPnoSV_ptReweighted_dataUseMCJPcalib_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 	
// 	}
// 
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_JPnoSV_ptReweighted_dataUseMCJPcalib_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_JPnoSV_ptReweighted_dataUseMCJPcalib_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 	
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 		
// 	}


// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_SVmass_ptReweighted_dataUseMCJPcalib_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_SVmass_ptReweighted_dataUseMCJPcalib_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 	
// 	}
// 
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_SVmass_ptReweighted_dataUseMCJPcalib_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_SVmass_ptReweighted_dataUseMCJPcalib_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 	
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 		
// 	}
*/

//Apr3-2018 =============================
/*
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_JPhasSV_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_JPhasSV_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 	
// 	}
// 
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_JPhasSV_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_JPhasSV_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 	
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 		
// 	}

// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_JPnoSV_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_JPnoSV_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 	
// 	}
// 
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_JPnoSV_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_JPnoSV_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 	
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 		
// 	}

// 
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_SVmass_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_SVmass_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 	
// 	}
// 
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_SVmass_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_SVmass_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 	
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 		
// 	}
*/


//Mar27-2018 =============================
/*
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_JPhasSV_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_JPhasSV_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 	
// 	}
// 
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_JPhasSV_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_JPhasSV_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 	
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 		
// 	}

// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_JPnoSV_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_JPnoSV_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 	
// 	}
// 
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_JPnoSV_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_JPnoSV_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 	
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 		
// 	}


// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_SVmass_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_SVmass_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 	
// 	}
// 
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_SVmass_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_SVmass_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 	
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 		
// 	}
*/

//Mar22-2018 per era =============================
/*

// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017EF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v3_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017EF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v3_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 	
// 	}
// 
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017EF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v3_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017EF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v3_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 	
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 		
// 	}
// 
// 
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017CDE_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v3_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017CDE_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v3_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 	
// 	}
// 
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017CDE_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v3_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017CDE_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v3_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 	
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 		
// 	}
// 
// 
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017B_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v3_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017B_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v3_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 	
// 	}
// 
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017B_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v3_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017B_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v3_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 	
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 		
// 	}
*/

//Mar15-2018 =============================
/*
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v3_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v3_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 	
// 	}
// 
// 	{
// 
// 	fin = "/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v3_ptReweighted_SysMerged.root";
// 	fout = "SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v3_ptReweighted_SysMerged_SFtemplates";
// 	std::cout << "Processing to convert double-Mu-tag file ... " << fin << std::endl; 
// 	
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,"H",kScales,kScaleStr);
// 	convert(fin,fout,"M1",kScales,kScaleStr);
// 	convert(fin,fout,"M2",kScales,kScaleStr);
// 	convert(fin,fout,"L",kScales,kScaleStr);
// 	}
// 		
// 	}
*/


	std::cout << "================"<< std::endl;
	std::cout << "Processing 350" << std::endl;
	std::cout << "================"<< std::endl;
	
	fin.push_back("/.automount/home/home__home1/institut_3a/novak/btag/CMSSW_9_4_13/src/RecoBTag/BTagValidation/test/results/plots_final/Run2017_QCDMuEnriched_DoubleMuonTaggedFatJets_histograms_btagval.root");
	fout.push_back("SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v3_ptReweighted_SysMerged_SFtemplates");
	varName.push_back("JP"); //JP must always be first. (Because all histo scaling is based on JP pre tag histos)

	// fin.push_back("/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_JPhasSV_ptReweighted_SysMerged.root");
	// fout.push_back("SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_JPhasSV_ptReweighted_SysMerged_SFtemplates");
	// varName.push_back("JPhasSV");

	// fin.push_back("/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_JPnoSV_ptReweighted_SysMerged.root");
	// fout.push_back("SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_JPnoSV_ptReweighted_SysMerged_SFtemplates");
	// varName.push_back("JPnoSV");

	// fin.push_back("/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_SVmass_ptReweighted_SysMerged.root");
	// fout.push_back("SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK8Jet300orAK4Jet300_Pt350_Final_DoubleMuonTaggedFatJets_histograms_btagval_v4_SVmass_ptReweighted_SysMerged_SFtemplates");
	// varName.push_back("tau1VertexMassCorr");

	{
	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
	kScaleStr = "";
	// convert(fin,fout,varName,"H",kScales,kScaleStr);
	convert(fin,fout,varName,"M1",kScales,kScaleStr);
	convert(fin,fout,varName,"M2",kScales,kScaleStr);
	convert(fin,fout,varName,"L",kScales,kScaleStr);
	}

	fin.clear();
	fout.clear();
	varName.clear();

// 	std::cout << "================"<< std::endl;
// 	std::cout << "Processing 250" << std::endl;
// 	std::cout << "================"<< std::endl;
// 
// 	fin.push_back("/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v3_ptReweighted_SysMerged.root");
// 	fout.push_back("SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v3_ptReweighted_SysMerged_SFtemplates");
// 	varName.push_back("JP"); //JP must always be first. (Because all histo scaling is based on JP pre tag histos)
// 
// 	fin.push_back("/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_JPhasSV_ptReweighted_SysMerged.root");
// 	fout.push_back("SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_JPhasSV_ptReweighted_SysMerged_SFtemplates");
// 	varName.push_back("JPhasSV");
// 
// 	fin.push_back("/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_JPnoSV_ptReweighted_SysMerged.root");
// 	fout.push_back("SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_JPnoSV_ptReweighted_SysMerged_SFtemplates");
// 	varName.push_back("JPnoSV");
// 
// 	fin.push_back("/afs/cern.ch/user/r/rsyarif/workHere/HbbTagVal/Jan10-2018_CommSF_v1/CMSSW_9_4_1/src/RecoBTag/BTagValidation/BTV/results/plots_final/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_SVmass_ptReweighted_SysMerged.root");
// 	fout.push_back("SFtemplates/"+date+"/Run2017BCDEF_ReReco_QCDMuonEnriched_AK4DiJet170_Pt250_Final_DoubleMuonTaggedFatJets_histograms_btagval_v1_v4_SVmass_ptReweighted_SysMerged_SFtemplates");
// 	varName.push_back("tau1VertexMassCorr");
// 
// 	{
// 	double kScales[7] = {1.,1.,1.,1.,1.,1.,1.}; //nominal
// 	kScaleStr = "";
// 	convert(fin,fout,varName,"H",kScales,kScaleStr);
// 	convert(fin,fout,varName,"M1",kScales,kScaleStr);
// 	convert(fin,fout,varName,"M2",kScales,kScaleStr);
// 	convert(fin,fout,varName,"L",kScales,kScaleStr);
// 	}


	std::cout << "Done. " << std::endl; 
	std::cout << "" << std::endl; 
	
	
	gApplication->Terminate();

}