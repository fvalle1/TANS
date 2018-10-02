#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#endif

using namespace std;


void io_0(const char * inFilename, const char* outFilename){
    ifstream infile(inFilename);
    ofstream outfile(outFilename);

    if(!infile){
        cerr<<"File"<< inFilename <<"not found"<<endl;
        return;
    }

    for(int i=0; i<1000;i++){
        outfile<<i<<endl;
    }

    int n;

    while(infile>>n){
        cout<<n<<endl;
    }

    infile.close();
    outfile.close();
}