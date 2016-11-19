#for fichier in Areizaga/*.txt
do
    echo $fichier
    julia AreizagaOR.jl $fichier
done
