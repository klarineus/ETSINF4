#!/usr/bin/octave -qf

if (nargin!=5)
printf("Usage: mlp-eva.m <trdata> <trlabels> <tedata> <telabels> <nHiddens>\n")
exit(1);
end;

arg_list=argv();
trdata=arg_list{1};
trlabs=arg_list{2};
tedata=arg_list{3};
telabs=arg_list{4};
nHiddens=str2num(arg_list{5});

load(trdata);
load(trlabs);
load(tedata);
load(telabs);

addpath("nnet_apr");

printf("\npca nH  dv-err");
printf("\n--- --- ------\n");

N=rows(X);
seed=23; rand("seed",seed); permutation=randperm(N);
X=X(permutation,:); xl=xl(permutation,:);

trper = 90;
dvper = 10;
Ntr=round(trper/100*N);
Ndv=round(dvper/100*N);
Xtr=X(1:Ntr,:); xltr=xl(1:Ntr);
Xdv=X(N-Ndv+1:N,:); xldv=xl(N-Ndv+1:N);

[m W]=pca(X);
Xtr=Xtr-m;
Xdv=Xdv-m;
Y=Y-m;

show=10;
epochs=300;
pcaKs=[20];
seed=23;
rand("seed",seed);

for k=1:length(pcaKs)
    pcaXtr= Xtr * W(:,1:pcaKs(k));
    pcaXdv= Xdv * W(:,1:pcaKs(k));
    pcaY = Y * W(:,1:pcaKs(k));
    for i=1:length(nHiddens)
    	edv = mlp(pcaXtr,xltr,pcaXdv,xldv,pcaY,yl,nHiddens(i),epochs,show,seed);
   	pause(10);
   	%printf("%6.3f\n", edv);
      	m=edv/100;
      	s=sqrt(m*(1-m)/rows(Y));
      	r=1.96*s;
      	printf("%3d %3d %6.3f [%.3f,%.3f]\n",pcaKs(k),nHiddens(i),m*100,(m-r)*100,(m+r)*100);
    end
end
