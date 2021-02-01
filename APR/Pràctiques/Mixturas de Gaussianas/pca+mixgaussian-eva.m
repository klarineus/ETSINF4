#!/usr/bin/octave -qf

if (nargin!=4)
printf("Usage: pca+mixgaussian-eva.m <trdata> <trlabels> <tedata> <telabels>\n")
exit(1);
end;

arg_list=argv();
trdata=arg_list{1};
trlabs=arg_list{2};
tedata=arg_list{3};
telabs=arg_list{4};

load(trdata);
load(trlabs);
load(tedata);
load(telabs);

%
% HERE YOUR CODE
%

alphas=[1e-4];
pcaKs=[50 100];
Ks=[50];

printf("\n  alpha pca  Ks   dv-err");
printf("\n------- ---  ---  ------\n");

[m W]=pca(X);
Xtr=X-m;
Xdv=Y-m;

for i=1:length(alphas)
  for k=1:length(pcaKs)
    %pcaXtr= Xtr * W(:,1:pcaKs(k));
    %pcaXdv= Xdv * W(:,1:pcaKs(k));
    for j=1:length(Ks)
      edv=mixgaussian(pcaXtr, xl, pcaXdv, yl, Ks(j), alphas(i));
      printf("%.1e %3d %3d %6.3f\n",alphas(i), pcaKs(k),Ks(j), edv);
      m=edv/100;
      s=sqrt(m*(1-m)/rows(Xdv));
      r=1.96*s;
      printf("%3d %3d %6.3f [%.3f,%.3f]\n",pcaKs(k),Ks(j),m*100,(m-r)*100,(m+r)*100);
    end
  end
end
