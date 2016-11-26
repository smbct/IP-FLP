for instance in Bench/*.dat
do
    echo $instance
    ./../main $instance >> resultatsBornes.txt
done
