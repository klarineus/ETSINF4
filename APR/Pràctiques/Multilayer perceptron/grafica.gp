#!/usr/bin/gnuplot -persist
set terminal postscript eps font "Helvetica,24" color
set output "pcamlp-exp.eps"
set logscale x
set logscale y
set xrange [0.9:100]
set yrange [3:100]
set xtics ("1" 1, "2" 2, "5" 5, "10" 10, "20" 20,"30" 30,"40" 40,"50" 50)
set ytics ("2" 2, "5" 5, "10" 10, "20" 20,"40" 40, "50" 50, "70" 70,"100" 100)
set label "nHidden" at 1,4 left
set label "Error (%)" at 0.92,85 left
set label "MNIST PCA+Mlp-exp" at 20,40 center font "HelveticaBold,20"
set terminal postscript eps font "Helvetica,15"
plot 'res.txt' u 1:2  t "1" w lp lw 2 lt 1 lc rgb '#00bcf2' ps 2.0,\
'res.txt' u 1:3  t "2" w lp lw 2 lt 1 lc rgb '#ff8c00' ps 2.0,\
'res.txt' u 1:4  t "5" w lp lw 2 lt 1 lc rgb '#e81123' ps 2.0,\
'res.txt' u 1:5  t "10" w lp lw 2 lt 1 lc rgb '#bad80a' ps 2.0,\
'res.txt' u 1:6  t "20" w lp lw 2 lt 1 lc rgb '#68217a' ps 2.0,\
'res.txt' u 1:7  t "50" w lp lw 2 lt 1 lc rgb '#00188f' ps 2.0
