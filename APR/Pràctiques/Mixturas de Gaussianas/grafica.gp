#!/usr/bin/gnuplot -persist
set terminal postscript eps font "Helvetica,24" color
set output "pcamixexpalfa5.eps"
set logscale x
set logscale y
set xrange [0.9:200]
set yrange [1.5:100]
set xtics ("1" 1, "2" 2, "5" 5, "10" 10, "20" 20, "50" 50, "100" 100)
set ytics ("1" 1, "2" 2, "5" 5, "10" 10, "20" 20,"40" 40, "50" 50, "70" 70,"100" 100)
set label "Mixtura  clasificador" at 1,3 left
set label "Error (%)" at 0.92,85 left
set label "MNIST PCA+MixGaussian a=1e-5" at 20,50 center font "HelveticaBold,20"
set terminal postscript eps font "Helvetica,15"
plot 'res5.txt' u 1:2  t "1" w lp lw 2 lt 1 lc rgb '#fff100' ps 2.0,\
'res5.txt' u 1:3  t "2" w lp lw 2 lt 1 lc rgb '#ff8c00' ps 2.0,\
'res5.txt' u 1:4  t "5" w lp lw 2 lt 1 lc rgb '#e81123' ps 2.0,\
'res5.txt' u 1:5  t "10" w lp lw 2 lt 1 lc rgb '#bad80a' ps 2.0,\
'res5.txt' u 1:6  t "20" w lp lw 2 lt 1 lc rgb '#68217a' ps 2.0,\
'res5.txt' u 1:7  t "50" w lp lw 2 lt 1 lc rgb '#00188f' ps 2.0,\
'res5.txt' u 1:8  t "100" w lp lw 2 lt 1 lc rgb '#00bcf2' ps 2.0
