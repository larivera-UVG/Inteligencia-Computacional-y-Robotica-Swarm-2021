% Esta función recibe coordenadas en el grid y devuelve el nodo al que
% corresponden las coordenadas

function Nodo = findNode(G,x,y)
h = G.Nodes.X==x;
k = G.Nodes.Y==y;
Nodo = find(h&k);
end

