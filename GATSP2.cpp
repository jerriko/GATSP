#include "GATSP_GAinit.h"
#include "GAFuncs.h"

vector<long> twoOpt (vector<long> tour) {
	//cout << "Entering 2opt" << endl;
	vector<long> opt;
	vector<long> ropt;
	for (long i = 1; i<vsize-2;i++) {
		for (long j = i+1; j<vsize-1;j++) {
			opt.push_back(tour[0]);
			opt.insert(opt.begin()+1,tour.begin() + 1, tour.begin()+i);
			//prTour(opt);
			for (long k = j; k>i-1; k--) {
				ropt.push_back(tour[k]);
			}
			opt.insert(opt.begin()+i,ropt.begin(),ropt.end());
			ropt.clear();
			opt.insert(opt.begin() + j+1,tour.begin()+j+1,tour.end()-1);
			opt.push_back(tour.at(vsize));
			//prTour(opt);
			if(fitness(opt)<fitness(tour)) {	
				tour = opt;
			}
			opt.clear();
		}
	}
	return tour;
}

vector<vector<long> > twoOptGen (vector<vector<long> > Gen) {
	for (long g = 0;g<GENERATION_SIZE;g++) {
		Gen[g] = twoOpt(Gen[g]);				
	}
	return Gen;
}

vector<vector<long> > dupeCheck(vector<vector<long> > Gen) {
	
	for (int i = 0; i <GENERATION_SIZE-1; i++) {
		for (int j = i; j < GENERATION_SIZE;j++) {
			if (Gen[i] == Gen[j] && i!=j) {
				Gen[j] = randTour(fitness(Gen[i]));
				//cout << "dupe found" <<endl;
			}
		}
	}
	
	return Gen;
}
void setArgs(int argc, char* argv[]) {
	if (argc < 3)
		exit(1);
	cout << "you entered " << argc << " arguments"<< endl;
	cout << "your arguments are: " << endl;
	for (int i = 1;i<argc; i+=2) {
		Function* func = functions.getFunction(argv[i]);
		cout << "command: " << (*func)(argv[i+1]) << endl;
	}
}
int main(int argc, char* argv[]) {
	
	srand((unsigned)time(0));
	//initialize environment
	/**************************************/
	//map and file variables
	setArgs(argc,argv);
	for (i = 0; i <vsize;++i) 
		weights[i] = new long[2];
	ofstream myfile;
	myfile.open("tours.txt");
	ifstream tsp;
	getMap();
	
	//execute Genesis function, recognize elite
	vector<vector<long> > Gen = Genesis();
	long eliteFit = fitness(track);
	champTour = Gen[0];
	
	//time variables
	
	clock_t t;
	clock_t t1;
	t = clock();
	/**************************************/
	
	
	//Selection process
	/**************************************/
	//cout << "Calculating.";
	for (long r = 0; r <RUNS ;r++) {
		
		if((r%100)==(RUNS%100))
			cout << "." << endl;
		//run fitness functions. looks messy, but scope requires it stays here :/ 
		for (i = 0; i < Gen.size(); i++) {
			fit[i] = fitness(Gen[i]);
			
			//write data to file for observation
			if (fit[i] < eliteFit) {
				eliteFit = fit[i];
				eliteTour = i;
				champTour = Gen[i];
				t1=clock()-t;
				myfile << "Best tour: " << eliteTour << " Run: " << r << " Fit: " << eliteFit << " Time: " << t1 << endl;
				for (n = 0; n < vsize+1; n++)
					myfile <<  champTour.at(n) << ", ";
				myfile <<  endl;

				//cout << t1 << " " << eliteFit << endl;
			}
		}
		//sort our population from most to least fit
		BubbleSort(fit);
		
		//we may want an infrequent immigration to prevent *too much* fresh genes
		//and 2opt takes a long time
		if (r%OPTFREQ == RUNS%OPTFREQ) {
			immigrate = true;
			//Gen = twoOptGen(Gen);
			Gen = dupeCheck(Gen);
		}
		//run the actual selection function
		Gen = Selection(Gen);
	}
	
	
	/**************************************/

	//wrap the program by twoOpting the elite individual and showing the results
	//champTour=twoOpt(champTour);
	eliteFit=fitness(champTour);
	cout << "Best tour: " << eliteTour << " Fit: " << eliteFit << endl;
	cout << "Path: ";
	prTour(champTour);
	writeTour(champTour);
	return 0;
}