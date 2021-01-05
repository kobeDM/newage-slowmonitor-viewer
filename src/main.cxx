#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include "Root_h.h"
#include "main.h"
#define MAXFILENUM 1000
#define MAXCH 128
#define PADCOL_DEFAULT 4
#define PADROW_DEFAULT 4
#define T_RANGE_UPDATE_DEFAULT 7200
#define INTERVAL 30
#define COLOR_NORMAL 30
#define COLOR_WARNING 49
#define CANVAS_X_DEFAULT 1500
#define CANVAS_Y_DEFAULT 890
#define PRESCALE 10
#define DATELIMIT 100

using namespace std;

#define DEBUG 0

void SetAxisProperties(TH2D *hist){
  hist->GetXaxis()->SetTimeDisplay(1);
  hist->GetXaxis()->SetTimeFormat("#splitline{%m/%d}{%H:%M}");
  hist->GetXaxis()->SetTimeOffset(32400,"gmt");
}

//int CalcDate(int n,tm *timer)
//{
//char ymd[9];
// //  time_t timer;
//struct tm *t_st;
  /* 現在時刻取得 */
  //  time(&timer);
  /* 1日の秒数86400秒 * n日を引く */
  //cout << timer <<endl;
  //timer += (86400 * n);
// cout << timer << endl;
  /* 時刻を構造体に変換 */
  //t_st = localtime(&timer);
  /* 文字列に書き込む */
  //sprintf(ymd, "%d%02d%02d", t_st->tm_year + 1900
//                       , t_st->tm_mon + 1
//                         , t_st->tm_mday);
// return atoi(ymd);
//}

int main(int argc, char *argv[]){
  cout<<"monitor [config_file] [start_date] [start_time] [end_date] [end_time]"<<endl;
  cout<<"monitor [config_file] 0 (for updating)";
  int i,j,id,plot,multi;
  int channels,pad;
  int padcol=PADCOL_DEFAULT;
  int padrow=PADROW_DEFAULT;
  int padmax=padcol*padrow;
  int canvas_x=CANVAS_X_DEFAULT;
  int canvas_y=CANVAS_Y_DEFAULT;
  int prescale=1;
  int date_limit=DATELIMIT;
  time_t utime_s,utime_e;
  time_t utime_n;
  struct tm tm;
  struct tm *tp,*te,*ts;
  FILE *fp;
  int t_range_update=T_RANGE_UPDATE_DEFAULT;
  // read options
  char date_n[128], date_s[128], date_e[128], time_n[128], time_s[128], time_e[128], dummy[128];
  char detector_name[128],info_file[128];
  char status_home[128], rate_home[128], CAEN_status_home[128], ups_status_home[128];
  char this_home[128],this_file[128];
  char can_title[256];

  //  int info_exists;
  string buf,buf_s; 
  int val_IDs[MAXCH][4];//id, pad, fileID, column
  int rate_sw[MAXCH]={0}; // rate switch
  int CAEN_sw[MAXCH]={0}; // CAEN switch
  int ups_sw[MAXCH]={0}; // ups switch
  double val_cals[MAXCH][8];//slope, offset, min, max, lower_imit for caution, higher_limit for caution log plot_color
  double val[MAXCH];
  int interval=INTERVAL;
  int update_sw=0;
  
  int markerstyle=8;
 double markersize=0.5;

  string val_name[MAXCH];
  string val_unit[MAXCH];

  string str_status = "";
  string str_warning = "";
  char str_tmp[128];
  
  utime_n=time(NULL);
  tp=localtime(&utime_n);
  strftime(date_n,10,"%Y%m%d",tp);
  //  strftime(time_s,10,"%H:%M:%S",tp);
  //  cerr << "time_s "<<time_s << endl;
  //FILE *fp = popen("date +\"%Y%m%d\" | tr -d '\n' ","r"); 
  // fgets(date_n,sizeof(date_n),fp); 
  //pclose(fp);

  utime_e=utime_n+24*3600;
  te=localtime(&utime_e);
  strftime(date_e,10,"%Y%m%d",te);

  utime_s=utime_n-24*3600;
  ts=localtime(&utime_s);
  strftime(date_s,10,"%Y%m%d",ts);
  cout <<endl<< "today " << date_n<<endl;
  cout <<"monitot starts at " << date_s<<endl;
  cout << "monitor ends at " << date_e<<endl;
    //    sprintf(date_s, date_n);  
    //sprintf(date_s,"%d",atoi(date_n)-1);
  //cout << "new date"<<CalcDate(1,tp)<<endl;

  //sprintf(date_e,"%d",atoi(date_n)+1);


  //sprintf(date_s, "20151126");  
  //  sprintf(time_s, "00:00:00");  
  sprintf(time_s, "0");  
  sprintf(time_e, "0");  
  //  sprintf(date_e, "20151127");  
  //  sprintf(time_e, "00:00:00");
  sprintf(detector_name,"NEWAGE-0.3b");
  sprintf(status_home,"/home/msgc/status");
  sprintf(rate_home,"/home/msgc/rate");
  sprintf(CAEN_status_home,"/home/msgc/CAEN_status_na16");
  sprintf(ups_status_home,"/home/msgc/ups_status");
  sprintf(info_file,"/home/msgc/status/monitor/4.4.1/monitor_03b.cfg");
  //  cerr<< "date_s" <<date_s<<endl;
  //cerr<< "date_n" <<date_n<<endl;
  //cerr<< "date_e" <<date_e<<endl;
  
  
  if(argc>1){
    sprintf(info_file, argv[1]);
  }
 if(argc>2) sprintf(date_s, argv[2]);
  if(argc>3) sprintf(time_s, argv[3]);
  if(argc==5){ sprintf(date_e, "%d",atoi(argv[4])+1);    sprintf(time_e, "00:00:00"); }
  if(argc>=6){ sprintf(date_e, argv[4]);    sprintf(time_e, argv[5]);    }
  
//  cerr<<"date_n:"<<date_e<<endl;
//  cerr<<"date_s:"<<date_e<<endl;
//  cerr<<"date_e:"<<date_e<<endl;

 // reading info file
  if(atoi(date_s)==0){
    update_sw=1;
      if(atoi(time_s)>0){
	interval=atoi(time_s);
      }
      cerr << "updating mode, interval="<<interval<<"sec"<<endl;
  }


  // visualize
  #ifndef BGMODE
  TApplication app("app", &argc, argv);
  #endif
  //graphs declare
  TH2D *waku[MAXCH];
  TGraph *graph[MAXCH];
  TLine *l_limit_h[MAXCH],*l_limit_l[MAXCH];
  //  TPaveText *pt[4][channels]; 
  TPaveText *pt[4][MAXCH]; 

  plot=0;

  cerr<<"Reading config file " << info_file<<endl;
  ifstream data(info_file,std::ios::in);
  id=0;
  while( getline(data,buf) ){
    istringstream is(buf);  
    is>>buf_s;
    if(buf_s=="detector"){
      is>>buf_s;
      sprintf(detector_name,buf_s.c_str());
      cerr <<"detecor name: "<< detector_name<<endl;
    }
    else if(buf_s=="divide"){
      is>>buf_s;
      padrow=atoi(buf_s.c_str());
      is>>buf_s;
      padcol=atoi(buf_s.c_str());
    }
    else if(buf_s=="canvas"){
      is>>buf_s;
      canvas_x=atoi(buf_s.c_str());
      is>>buf_s;
      canvas_y=atoi(buf_s.c_str());

    }
  }
  // canvas declare
  if(update_sw)  sprintf(can_title,"status monitor (updating)");
  else  sprintf(can_title,"status monitor");


  cerr <<"canvas size:"<< canvas_x <<"*"<<canvas_y<<endl;
  cerr <<"pad division:"<< padrow <<"*"<<padcol<<endl;
  padmax=padcol*padrow;

  TCanvas *cc = new TCanvas("cc",Form("%s %s",detector_name,can_title),canvas_x,canvas_y);    
    cc->Divide(padcol,padrow, 0.0001,0.0001);
    
    //}
    gStyle->SetPalette(1);  
    gStyle->SetOptStat(0);
    gStyle->SetNdivisions(510,"x");      //root default is 510
    gStyle->SetNdivisions(507,"yz");     //root default is 510
    gStyle->SetNumberContours(99);       //root default is 20
    //    gStyle->SetLabelSize(0.038,"x");      //root default is 0.04
    gStyle->SetLabelSize(0.055,"x");      //root default is 0.04
    gStyle->SetLabelSize(0.06,"yz");     //root default is 0.04
    gStyle->SetLabelOffset(0.045,"x");   //root default is 0.005
    gStyle->SetLabelOffset(0.005,"yz");  //root default is 0.005
    gStyle->SetTitleSize(0.05,"xyz");    //root default is 0.02
    gStyle->SetTitleOffset(1.0,"x");     //root default is 1.0
    gStyle->SetTitleOffset(0.4,"yz");    //root default is 1.0


  do{//loop for updating 

  cerr<<"Reading config file " << info_file<<endl;
  ifstream data(info_file,std::ios::in);
  
  id=0;
  while( getline(data,buf) ){
    istringstream is(buf);  
    is>>buf_s;
    if(buf_s=="detector"){
      is>>buf_s;
      sprintf(detector_name,buf_s.c_str());
      cerr <<"detecor name: "<< detector_name<<endl;
    }
    else if(buf_s=="status_home"){
      is>>buf_s;
      sprintf(status_home,buf_s.c_str());
      cerr <<"status_home"<< status_home<<endl;
    }
    else if(buf_s=="rate_home"){
      is>>buf_s;
      sprintf(rate_home,buf_s.c_str());
      cerr <<"rate_home:"<< rate_home<<endl;
    }
    else if(buf_s=="CAEN_status_home"){
      is>>buf_s;
      sprintf(CAEN_status_home,buf_s.c_str());
      cerr <<"CAEN_status_home:"<< CAEN_status_home<<endl;
    }
    else if(buf_s=="ups_status_home"){
      is>>buf_s;
      sprintf(ups_status_home,buf_s.c_str());
      cerr <<"ups_status_home:"<< ups_status_home<<endl;
    }
    else if(buf_s=="t_range_update"){
      is>>buf_s;
      t_range_update=atoi(buf_s.c_str());
      if(update_sw)
      cerr <<"time range for updating monitor: "<< t_range_update<<endl;
    }


    else if(buf_s.substr(0,1)=="#"){
      //comment
    }
    else if(atoi(buf_s.c_str())>=0&&atoi(buf_s.c_str())<padmax){
      //    else if(atoi(buf_s.c_str())>=0&&atoi(buf_s.c_str())<MAXCH){
      //      id=atoi(buf_s.c_str());
      val_IDs[id][0]=id;//atoi(buf_s.c_str());
      //      val_IDs[id][0]=
      //      is>>buf_s;
      val_IDs[id][1]=atoi(buf_s.c_str());//pad
      is>>buf_s;
      val_name[id]=buf_s;
      //      cerr <<"subst" << buf_s.substr(0,4) <<endl;
      if(buf_s.substr(0,4)=="rate"||buf_s.substr(0,4)=="disk"){
	//	cerr << "id" << id <<" is rate cahnnel."<<endl;
	rate_sw[id]=1;
      }
      if(buf_s.substr(0,4)=="CAEN"){
        //      cerr << "id" << id <<" is CAEN cahnnel."<<endl;                                                                                  
        CAEN_sw[id]=1;
      }
      if(buf_s.substr(0,3)=="UPS"){
        //      cerr << "id" << id <<" is ups cahnnel."<<endl;
	ups_sw[id]=1;
      }

      is>>buf_s;
      val_unit[id]=buf_s;
      
      for(i=2;i<4;i++){
	is>>buf_s;
	val_IDs[id][i]=atoi(buf_s.c_str());
      }

      //}
      for(i=0;i<8;i++){
	is>>buf_s;
	val_cals[id][i]=atof(buf_s.c_str());
      }
    
       //id=val_name[id];
      cerr <<"plot ID "<<id<< " "<< val_name[id]<< " unit:["<< val_unit[id]<<"]"<<endl;
      cerr << "    conversion:" << val_cals[id][0]<<"*(value[V] -" <<val_cals[id][1]<<")" << endl;
      cerr << "    y range: " << val_cals[id][2]<< " to "<< val_cals[id][3] << val_unit[id]<<endl;
      cerr << "    lower limit: " << val_cals[id][4]<< ", upper limit: "<< val_cals[id][5] << val_unit[id]<<endl;
      
      cerr << "    plot: pad "<< val_IDs[id][1];
      if(val_cals[id][6])       cerr << " log, " ;
      else       cerr << " linear, ";
      cerr << "color  " << val_cals[id][7]<<endl;
      id++;
    }
  }// config file reading end
  channels=id;


    
    // time range is got from date command
    fp = popen(Form("date +%%s --date \"%s %s\"",date_s,time_s),"r"); 
    fgets(dummy,sizeof(dummy),fp); 
    pclose(fp); 
    utime_s = atoi(dummy);
    
    fp = popen(Form("date +%%s --date \"%s %s\"",date_e,time_e),"r"); 
    fgets(dummy,sizeof(dummy),fp); 
    pclose(fp); 
    utime_e = atoi(dummy);
    //cerr << "updating loop starts" <<endl;
    //   do{//loop for updating 
      if(update_sw){
	// time range is automatically determined
	utime_e=time(NULL);
	utime_s=utime_e-t_range_update;
	//	utime_s=utime_e-1800;
	tp=localtime(&utime_s);
	strftime(date_s,10,"%Y%m%d",tp);
	strftime(time_s,10,"%H:%M:%S",tp);
	tp=localtime(&utime_e);
	strftime(date_e,10,"%Y%m%d",tp);
	strftime(time_e,10,"%H:%M:%S",tp);
      }
      if(DEBUG)      cerr <<"utime_s "<< utime_s  <<" date_s "<< date_s <<endl;
       if(DEBUG) cerr <<"utime_e "<< utime_e<<" date_e "<< date_e <<endl;
       if(DEBUG) cerr <<"utime_n "<< utime_n <<endl;

  // define graphs
  for(id=0;id<channels;id++){
    //    if(DEBUG) cerr<< "utimes" << utime_s<<" "<<utime_e<<endl;
    if(DEBUG) cerr<< "id=" << id;
    if(plot){
      delete waku[id];
      if(DEBUG) cerr<< "waku delete ok. " ;
    }
    waku[id]  = new TH2D(Form("waku_%s",val_name[id].c_str()), Form(";;%s",val_unit[id].c_str()),  1000,utime_s,utime_e, 1000,val_cals[id][2],val_cals[id][3]);  
    SetAxisProperties(waku[id] );
    //    waku[id]->GetXaxis()->SetLabelSize(0.1);
      //SetTimeFormat
  //#splitline{%m/%d}{%H:%M}%F1970-01-01 09:00:00s0 GMT
    if(DEBUG) cerr<< "waku ok. " ;

    graph[id]  = new TGraph();  
    graph[id] ->SetLineColor(int(val_cals[id][7])); 
    graph[id] ->SetMarkerColor(int(val_cals[id][7])); 
    graph[id] ->SetMarkerStyle(markerstyle); 
    graph[id] ->SetMarkerSize(markersize); 
    if(DEBUG) cerr<< "graph ok" ;

    l_limit_l[id]  = new TLine(utime_s,val_cals[id][4],  utime_e,val_cals[id][4]);  
    l_limit_h[id]  = new TLine(utime_s,val_cals[id][5],  utime_e,val_cals[id][5]);  
    l_limit_l[id] ->SetLineColor(4);
    l_limit_h[id] ->SetLineColor(2);
    if(DEBUG) cerr<< "line ok" <<endl;
    
  }
  
  
   if(DEBUG) cerr << "from " << date_s <<" "<<time_s<<endl;
   if(DEBUG) cerr << "to " << date_e <<" "<<time_e<<endl;
  // read files
  int index=0, year, month, day, file_exist; 

  for(id=0;id<channels;id++){
    if(DEBUG)    cerr <<"reading channel "<< id <<"/"<<channels;
    if(atoi(date_e)-atoi(date_s)>date_limit){
      prescale=PRESCALE;
      cerr<<"time range is large."<<endl<< "prescale by "<<prescale<<endl;
    }
    for(int i=atoi(date_s); i<=atoi(date_e); i++){
      //    cerr << "reading "<<i<<" id="<<id<<endl;
      year = i/10000;  month = (i-year*10000)/100;  day = i-year*10000-month*100;
      if(month<=12 && day<=31){//pre-select file
      if(rate_sw[id])
	sprintf(this_home,"%s",rate_home);
      else if(ups_sw[id])
        sprintf(this_home,"%s",ups_status_home);
      else if(CAEN_sw[id])
	sprintf(this_home,"%s",CAEN_status_home);
      else      
	sprintf(this_home,"%s",status_home);
      if(val_IDs[id][2]>0&&!(rate_sw[id]))
	sprintf(this_file,"%s/%d_%d",this_home,i,val_IDs[id][2]);
      else
	sprintf(this_file,"%s/%d",this_home,i);
      
      //      if(!(fp = popen(Form("if [ -e %s/%d ]; then echo 1; else echo 0; fi",this_home,i),"r"))){
      if(!(fp = popen(Form("if [ -e %s ]; then echo 1; else echo 0; fi",this_file),"r"))){
	cerr<<"status file " << this_file<<"/"<<i<< " does not exit."<<endl;
      }
      else{// file open OK
	pclose(fp); 
      	//if(prescale>1)
	//cerr << "reading file " << this_file <<endl;
	//	ifstream data(Form("%s/%d",this_home,i),std::ios::in);
	ifstream data(Form("%s",this_file),std::ios::in);
	while( getline(data,buf) ){// data read
	  istringstream is(buf);  
	  //	  stringstream buf_c(buf.c_str());
	  is>>buf_s;
	  //cerr <<"buf_s:"<<buf_s ;
	  strptime(buf_s.c_str(),"%Y/%m/%d/%H:%M:%S", &tm);
	  	  utime_n = mktime(&tm);
	  //	  cerr  <<  "utime_n " << utime_n<<endl;
	  //cerr  <<  "utime_s " << utime_s<<endl;
	  //cerr  <<  "utime_e " << utime_e<<endl;
	  	  
	  if(utime_s<=utime_n && utime_n<=utime_e){//fill
	    char date[128], time[128];  
	    double valueV[MAXCH];  
	    int vnum=0;
	    while(is >> valueV[vnum]) {
	      //	     cerr << valueV[vnum]<<" ";
	      vnum++;
	    }
	    val[id]= val_cals[id][0]*(valueV[val_IDs[id][3]-2]-val_cals[id][1]);
	    graph[id] ->SetPoint(index, utime_n, val[id] );
	    index++;
	  }
	  //}
	  for(j=0;j<prescale-1;j++)getline(data,buf);
	}
	
	//	if(prescale>1)cerr<< "done." << endl;
      }
      }
    }
    cerr << " done\r";
  }
    cerr << "\n";

  // pavetext
  //system("if [ -e status.txt ]; then rm status.txt ; fi");
  //system("if [ -e warning.txt ]; then rm warning.txt ; fi");

  TPaveText *pt_info = new TPaveText(0,0.5, 1,1, ""); 
  pt_info->SetFillColor(0);
  TPaveText *pt_info_sub = new TPaveText(0,0, 1,.5, ""); 
  pt_info_sub->SetFillColor(0);
  TPaveText *pt_info_detail = new TPaveText(0,0.1, .95,.6, ""); 
  pt_info_detail->SetFillColor(0);
  // TPaveText *pt_dummy = new TPaveText(0,0.1, .95,.6, ""); 
  //dummyborder->SetFillColor(0);

  char datetime[128];  double xx, yy;
  yy=0;
  fp = popen("date +\"%Y/%m/%d %H:%M:%S\" | tr -d '\n' ","r"); 
  fgets(datetime,sizeof(datetime),fp); pclose(fp);
  pt_info->AddText(Form("%s status monitor",detector_name));
  if(update_sw)
  pt_info_sub->AddText(Form("(updating mode, interval %d sec)",interval));
  pt_info_sub->AddText(Form("created at %s",datetime));
  pt_info_detail->AddText(Form("config file: %s",info_file));
  pt_info_detail->AddText(Form("status data directory: %s",status_home));
  pt_info_detail->AddText(Form("rate data directory: %s",rate_home));
  pt_info_detail->AddText(Form("CAEN data directory: %s",CAEN_status_home));
  pt_info_detail->AddText(Form("ups data directory: %s",ups_status_home));
  pt_info_detail->AddText(Form("from  %s %s",date_s,time_s));
  pt_info_detail->AddText(0,0,Form(" to   %s %s",date_e,time_e));
  cerr<<"time range "<< date_s << " "<<time_s << "--"<< date_e << " "<<time_e <<endl;
  cerr<<"monitor updated at "<<datetime<<endl;
 
  //  cerr<<"making graph ok."<<endl;

  /**
 
  cout<<" status :::::::: "<<str_status<<endl;
  cout<<" warning ::::::: "<<str_warning<<endl;
  system(Form("echo \"%s\" > status.txt",str_status.c_str()));
  system(Form("echo \"%s\" > warning.txt",str_warning.c_str()));
  **/

  cc->SetFillColor(1);

  for(int i=1; i<=padmax; i++){ 
    cc->cd(i)->SetGrid(1);  
    //    cc->cd(i)->SetFillColor(10+9*(((i-1)/padrow)%2));  
    //    cc->cd(i)->SetShadowColor(1);  
    cc->cd(i)->SetFillColor(10);  
    // cc->cd(i)->SetLineColor(1);   
    //cc->cd(i)->SetLineWidth(3);  
    //cc->cd(i)->SetBorderMode(1);  
    //cc->cd(i)->SetMargin(0.07,0.15,0.15,0.05); } // lrbt
    //   cc->cd(i)->SetMargin(0.1,0.05,0.3,0.05); } // from 4.2
    cc->cd(i)->SetMargin(0.1,0.05,0.15,0.25); } // from 4.3
  if(padmax>2){
    cc->cd(1); 
    pt_info->Draw();
    pt_info_sub->Draw("same");
    cc->cd(padmax); 
    pt_info_detail->Draw();
  }
  for(id=0;id<channels;id++){
    /**
    pt[0][id]= new TPaveText(0.,0, 0.24,0.15, "NDC");  
    pt[1][id]= new TPaveText(0.25,0, 0.49,0.15, "NDC");  
    pt[2][id]= new TPaveText(0.5,0, 0.74,0.15, "NDC");  
    pt[3][id]= new TPaveText(0.75,0., 0.98,0.15, "NDC"); 
    **/
    pt[0][id]= new TPaveText(0.02,0.8, 0.25,.95, "NDC");  
    pt[1][id]= new TPaveText(0.26,0.8, 0.49,.95, "NDC");  
    pt[2][id]= new TPaveText(0.51,0.8, 0.74,.95, "NDC");  
    pt[3][id]= new TPaveText(0.75,0.8, 0.98,.95, "NDC");  

  }
  //  cerr<<"making pt ok."<<endl;

  for(pad=1;pad<padmax;pad++){
    //    dummyborder->Draw();
    multi=0;
    //cerr<<"pad:"<<pad ;//"plot id "<<id<<" in pad "<<pad<<endl;
    for(id=0;id<channels;id++){
      //cerr<<" id=" << id <<" IDs["<<id<<"][1]="<<val_IDs[id][1]<<" ";//"plot id "<<id<<" in pad "<<pad<<endl;
      //
      if(pad==val_IDs[id][1]){
	//cerr<<" id=" << id <<" IDs["<<id<<"][1]="<<val_IDs[id][1]<<" ";//"plot id "<<id<<" in pad "<<pad<<endl;
	//cerr << "matched."<<endl;
	if(val_cals[id][6]){
	cc->cd(val_IDs[id][1])->SetLogy();  
	//cerr<<"plot id "<<id<<" in pad "<<pad<<" log"<<endl;
	}
	else	cc->cd(val_IDs[id][1])->SetLogy(0);  

	waku[id]->Draw(); 
	//cerr<<id<<" waku ok";

		graph[id] ->Draw("same,P"); 
		//graph[id] ->Draw("same,L"); 
	//cerr<<id<<" graph ok";
	l_limit_l[id]->Draw("same");
	l_limit_h[id]->Draw("same");
	//cerr<<id<<" limits ok";
	// cerr<<id<<" ";
	//message
	pt[multi][id]->SetFillColor(kBlue-10);
	pt[multi][id]->SetTextColor(int(val_cals[id][7]));
	graph[id]->GetPoint(graph[id]->GetN()-1,xx,yy);
	if(yy>val_cals[id][5]||yy<val_cals[id][4]){ 
	  pt[multi][id]->SetFillColor(kRed-10);  
	}
	pt[multi][id]->AddText(val_name[id].c_str());//.c_str());  
	pt[multi][id]->AddText(Form("%.2f %s", yy,val_unit[id].c_str()));
	pt[multi][id] ->Draw("same");
	break;
      }
    }
    //  cerr<<"plotting ok."<<endl;
  for(id=id+1;id<channels;id++){
    if(pad==val_IDs[id][1]){
      //  cerr<<"plot id "<<id<<" in pad "<<pad<<endl;
	//	waku[id]->Draw(); 
      //	graph[id] ->Draw("same,L"); 
		graph[id] ->Draw("same,P"); 
	// cerr<<id<<" ";
	l_limit_l[id]->Draw("same");
	l_limit_h[id]->Draw("same");
		graph[id] ->Draw("same,P"); 
		//graph[id] ->Draw("same,L"); 
	multi++;
	pt[multi][id]->SetFillColor(kBlue-10);
	pt[multi][id]->SetTextColor(int(val_cals[id][7]));
	graph[id]->GetPoint(graph[id]->GetN()-1,xx,yy);
	if(yy>val_cals[id][5]||yy<val_cals[id][4]){ 
	  pt[multi][id]->SetFillColor(kRed-10);  
	}
	pt[multi][id]->AddText(val_name[id].c_str());//.c_str());  
	pt[multi][id]->AddText(Form("%.2f %s", yy,val_unit[id].c_str()));
	// cerr<<id<<" ";
	pt[multi][id] ->Draw("same");
      }
     }
  }  // l_V_anode ->Draw("same"); 
  //g_V_anode ->Draw("sameL"); 
  //  cc->cd(5);  waku_V_anode ->Draw(); l_V_anode ->Draw("same"); g_V_anode ->Draw("sameL"); pt_V_anode ->Draw("same");
    //cc->cd(6);  waku_A_anode ->Draw(); l_A_anode ->Draw("same"); g_A_anode ->Draw("sameL"); pt_A_anode ->Draw("same");
  
      
  if(update_sw){  
    plot=1;
    cc->Update();
    cerr <<"1st plot done."<<endl;
    sleep(interval);
  }
  else{
  // save
    TFile *file_output = new TFile("status.root","RECREATE");
    cc->Write();
    delete file_output;
    cc->Print("status.png");
    cerr << "saved in status.png"<<endl;

#ifndef BGMODE
    app.Run();
#endif
  }
}while(update_sw);//loop for auto update
  cout<<" [monitor] end"<<endl;
  return 0;
}
