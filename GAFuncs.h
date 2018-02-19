#pragma once


vector<long> champ;
vector<long> champTour;
vector<vector<long> > rank;
vector<long> cross;

double fit[GENERATION_SIZE];
long v;
double pi = 3.141592;
double rad = pi/180;
long* newFit;

void prTour(vector<long> child) {
	for (long g = 0; g < child.size(); g++)
		cout << child.at(g) << ", ";
	cout << endl;
}

double fitness(vector<long> tour) {
	//cout << "Entering fitness function" <<endl;
	double sum = 0;
	double tourD;
	c1 = tour.front();
	temp=tour;
	double x1,x2,y1,y2;
	double deg,min,rad;
	double q1,q2,q3,dij;
	for (int jj=1;jj<tour.size();jj++) {
		tourD=0;
		c2 = tour[jj];
		x1 = weights[c1][0];
		x2 = weights[c2][0];
		y1 = weights[c1][1];
		y2 = weights[c2][1];
		q1 = cos(x1-x2);
		q2 = cos(y1-y2);
		q3 = cos(y1+y2);
		dij = 6378.388 * acos(0.5*((1.0+q1)*q2)-(1.0-q1)*q3)+1.0;
		//cout << "Computing " << c1 << " and " << c2 << endl;
		tourD =sqrt((pow(weights[c2][0]-weights[c1][0],2))+(pow(weights[c2][1]-weights[c1][1],2)));
		//cout << "Distance: " << tourD<<endl;
		sum += tourD;
		c1 = c2;
		
	}
	
	//cout << sum << endl;
	return sum;
}

long partSearch(vector<long> p, long c) {
	long pos = 0-1;
	long s = 0;
	while (pos!=c && s<vsize) {
		s++;
		pos=p[s];
	}
	
	return s;
}

vector<long> compat(vector<long> p1, vector<long> p2) {
	//cout << "Enter compat check" << endl;
	vector<long> crossover,part,best;
	long search,bestfit,crossfit,found,i1=1,i2=1;
	valid=false;
	bool running = true;
	if (p1[0] == p2[0]) {
		while (i2<vsize && running) {
			//cout << "Search " << i2 << ": " << endl;
			crossover.clear();
			part.clear();
			temp=track;
			//Flip the beginning of our partition:
			temp[p1[0]]= 0-1;
			//Assign the city we are searching for in p2 to match the city in p1:
			search=rand()%vsize;
			i1=1;
			//Begin search
			
			while(i1<(vsize/2)) {
				found = partSearch(p2,p1[search]);
				if (temp[found]==-1)
					i1=vsize;
				else {
					crossover.push_back(found);
					part.push_back(p2[found]);
					temp[found] = 0-1;
					search = partSearch(p1,p2[found]);
					i1=1;
					i2++;
				}
				i1++;
			}
			
			if (crossover.size() > 10)
				running = false;
		}
		if (crossover.size() > 10) { 
			valid=true;
		}
	}
	//cout << "End compat" << endl;
	return crossover;
}

vector<long> Mutate(vector<long> child) {
	//cout <<"Mutation" <<endl;
	vector <long> p;
	long mut1;
	long mut2;
	long r1 = 0;
	long ctemp;
	while (r1 < vsize/MUTE) {
		p = child;
		mut1 = rand()%(vsize-4)+2;
		mut2 = rand()%(vsize-4)+2;
		if (mut1==mut2) {
				if (mut2>=(vsize/2))
					mut2-=rand()%(vsize/2);
				else
					mut2+=rand()%(vsize/2);
		}
		ctemp = p[mut1];
		p[mut1] = p[mut2];
		p[mut2] = ctemp;
		if (fitness(child) < fitness(p)) {
			p[mut2]=p[mut1];
			p[mut1]=ctemp;
		}
		r1++;
	}

	//cout << "Mutation failed" << endl;
	return p;
}

vector<long> Mutate2(vector<long> child) {
	long temp1;
	long temp2;
	long ran1 = rand()%(child.size()-5)+1;
	long ran2 = rand()%(child.size()-5)+1;
	if (ran1!=ran2) {
		long tfit = fitness(child);
		for (int i = 0;i<2;i++) {
			temp1 = child[ran1+1];
			child[ran1+1] = child[ran2+i];
			child[ran2+i] = temp1;
			if (tfit>fitness(child))
				return child;
			else {
				child[ran2+i] = child[ran1+1];
				child[ran1+1] = temp1;
			}
		}
	}
	
	
	
	return child;
}

vector<long> Crossover(vector<long> p1,vector<long> p2, vector<long> strands) {
	vector<long> child;
	child = p1;
	long ctemp;
	for (k=0;k<strands.size();k++) {
		ctemp = p1[k];
		p1[k] = p2[k];
		p2[k] = ctemp;
	}
	if (fitness(child) < fitness(p1)) {
		return child;
	}
	else {
		//return Mutate2(p1);
		return p1;
	}
}

vector<vector<long> > Immigrate(vector<vector<long> > Gen) {
	long a = GENERATION_SIZE-(GENERATION_SIZE/5);
	long bound = getBound();
	for (int i = GENERATION_SIZE-1; i>a;i--) {
		Gen[i] = randTour(bound);
	}
	return Gen;
}

vector<vector<long> > Selection(vector<vector<long> > Gen) {
	if (immigrate) {
		Gen = Immigrate(Gen);
		immigrate = false;
	}
	//cout << "Entering Selection" << endl;
	vector<long> strands;
	vector<long> tempp;
	for (j=0;j<GENERATION_SIZE-1;j+=2) {
		strands = compat(Gen[temprank[j]],Gen[temprank[j+1]]);
		if(false) {
			tempp=Gen[temprank[j]];
			Gen[temprank[j]] = Crossover(Gen[temprank[j]],Gen[temprank[j+1]],strands);
			Gen[temprank[j+1]] = Crossover(Gen[temprank[j+1]],tempp,strands);
		}
		else {
			Gen[temprank[j]] = Mutate(Mutate2(Gen[temprank[j]]));
			Gen[temprank[j+1]] = Mutate(Mutate2(Gen[temprank[j+1]]));
		}
	}
	return Gen;
}

vector<vector<long> > Genesis() {
	cout<<"Init Gen" << endl;
	long bound = getBound();
	vector<vector<long> > *Gen = new vector<vector<long> >();
	for (k=0;k<GENERATION_SIZE;k++) {
		temprank[k] = k;
		(*Gen).push_back(randTour(bound));
	}
	cout << "Gen Initialized" << endl;
	return *Gen;
}