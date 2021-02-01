#!/usr/bin/octave -qf

if (nargin!=5)
printf("Usage: mlp-exp.m <trdata> <trlabels> <nHiddens> <%%trper> <%%dvper>\n")
exit(1);
end;

arg_list=argv();
trdata=arg_list{1};
trlabs=arg_list{2};
nHiddens=str2num(arg_list{3});
trper=str2num(arg_list{4});
dvper=str2num(arg_list{5});

load(trdata);
load(trlabs);
addpath("nnet_apr");

N=rows(X);
seed=23; rand("seed",seed); permutation=randperm(N);
X=X(permutation,:); xl=xl(permutation,:);

Ntr=round(trper/100*N);
Ndv=round(dvper/100*N);
Xtr=X(1:Ntr,:); xltr=xl(1:Ntr);
Xdv=X(N-Ndv+1:N,:); xldv=xl(N-Ndv+1:N);

printf("\n nH dv-err");
printf("\n--- ------\n");

show=10;
epochs=300;
for i=1:length(nHiddens)
  edv = mlp(Xtr,xltr,Xdv,xldv,Xdv,xldv,nHiddens(i),epochs,show,seed);
  printf("%3d %6.3f\n",nHiddens(i),edv);
end
