#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"


#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include <sstream>
#include <iomanip> 
#include <utility>


using namespace std;


//object definition
class Analysis : public edm::one::EDAnalyzer<> {
public:

  //constructor, function is called when new object is created
  explicit Analysis(const edm::ParameterSet& conf);

  //destructor, function is called when object is destroyed
  ~Analysis();

  //edm filter plugin specific functions
  virtual void beginJob();

  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

private:

  edm::ParameterSet theConfig;
  unsigned int theEventCount;

  TH1D *hBsPt;

  edm::EDGetTokenT < vector<reco::GenParticle> > theGenParticleToken;


};


Analysis::Analysis(const edm::ParameterSet& conf)
  : theConfig(conf), theEventCount(0)
{
  cout <<" CTORXX" << endl;

  theGenParticleToken = consumes< vector<reco::GenParticle>  >( edm::InputTag("genParticles" ));

}

Analysis::~Analysis()
{
  cout <<" DTOR" << endl;
}

void Analysis::beginJob()
{
  //create a histogram
  hBsPt = new TH1D("hBsPt","Transverse momentum of B_{s}^{0}; p_{t} [GeV]; Counts",50, 0., 100.);


  cout << "HERE Analysis::beginJob()" << endl;
}

void Analysis::endJob()
{
  //make a new Root file
  TFile myRootFile( theConfig.getParameter<std::string>("outHist").c_str(), "RECREATE");

  //write histogram data
  hBsPt -> Write();

  myRootFile.Close();

  delete hBsPt;


  cout << "HERE Mgr::endJob()" << endl;
}


void Analysis::analyze(
    const edm::Event& ev, const edm::EventSetup& es)
{
  std::cout << " -------------------------------- HERE Mgr::analyze "<< std::endl;
  //bool debug = true;

  const std::vector<reco::GenParticle> & genPar = ev.get(theGenParticleToken);
  std::cout << "Number of particles: " << genPar.size() << std::endl;
  //for (std::vector<reco::GenParticle>::const_iterator part = genPar.begin(); part < genPar.end(); part++) {}
  int nBs = 0;
  for (const auto& part:genPar ){
    
    if (abs(part.pdgId())/100 == 5){
      
      if (abs(part.pdgId()) == 531)nBs++;
      std::cout << part.pdgId() << std::endl;
      hBsPt -> Fill(part.pt());
      
    }
    //std::cout << "Id: " << part.pdgId() << std::endl;
  }
  cout <<"Number of Bs mesons: " << nBs << endl;
  cout <<"*** Analyze event: " << ev.id() <<" analysed event count:" << ++theEventCount << endl;
}

DEFINE_FWK_MODULE(Analysis);

