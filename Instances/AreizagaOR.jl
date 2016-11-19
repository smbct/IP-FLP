
nom = ARGS[1]

f = open(nom)

lines = readlines(f)

data = []

for line in lines
    liste = split(line)
    for elt in liste
        push!(data, parse(Float64,elt))
    end
end

#print(data)

close(f)

nbClients = round(Int64, data[1])
nbServices = round(Int64, data[2])

couts = []
capa = []

demandes = []

# matrice client*services
liaisons = rand(nbClients, nbServices)

#récupération des coûts de liaison
indice = 3
for i = 1:nbClients
    for j = 1:nbServices
        liaisons[i,j] = data[indice]
        indice += 1
    end
end

#récupération des demandes
for i = 1:nbClients
    push!(demandes, data[indice])
    indice += 1
end

#récupération des coûts d'ouverture
for i = 1:nbServices
    push!(couts, data[indice])
    indice += 1
end

#récupération des capacités des services
for i = 1:nbServices
    push!(capa, data[indice])
    indice += 1
end

#enregistrement dans un fichier
nom2 = split(nom, ".")[1]
nom2 = nom2*".dat"
sortie = open(nom2, "w")

print(sortie, nbServices)
print(sortie, " ")
println(sortie, nbClients)

# enregistrement capacité/coûts pour les services
for i = 1:nbServices
    print(sortie, capa[i])
    print(sortie, " ")
    println(sortie, couts[i])
end

# enregistrement des demandes
for i = 1:nbClients
    print(sortie, demandes[i])
    if i < nbClients
        print(sortie, " ")
    else
        print(sortie, "\n")
    end
end

# enregistrement des coûts de liaison
for i = 1:nbServices
    for j = 1:nbClients
        print(sortie, liaisons[j,i])
        if j < nbClients
            print(sortie, " ")
        else
            print(sortie, "\n")
        end
    end
end

close(sortie)
