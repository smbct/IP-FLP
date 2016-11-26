for instance in Bench/*.dat
do
    echo $instance
    ./../main $instance >> resultatsTempsBornes.txt
done
