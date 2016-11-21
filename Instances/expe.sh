for instance in Bench/*.dat
do
    echo $instance
    ./../solverGlpk $instance >> resultats.txt
done
