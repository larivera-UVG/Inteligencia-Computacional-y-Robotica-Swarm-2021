val = 0;
cont = 0;
for i = 1:size(var,1)
    if var(i,1) == 1
        cont = cont+1;
        val(cont) = var(i,2);
    end
end

tabulate(val)
% bar(val.Value,val.Count)
% xlabel('Country of Origin')
% ylabel('Number of Cars')