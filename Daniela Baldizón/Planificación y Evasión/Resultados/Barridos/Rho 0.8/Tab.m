val = 0;
cont = 0;
aumento = aumento + 0.2;
for i = 1:size(var,1)
    if var(i,1) == 100
        cont = cont+1;
        val(cont) = var(i,2);
    end
end

tabulate(val)
% bar(val.Value,val.Count)
% xlabel('Country of Origin')
% ylabel('Number of Cars')

%%
bar(var,val);
xlabel('Hormigas')
xticks(5:5:100)
ylabel('Frecuencia de costo mínimo')
