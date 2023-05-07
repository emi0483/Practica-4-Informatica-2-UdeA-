#include "router.h"

router::router(int newID){ ID = newID; } // Constructor

// Assign new routing table
void router::setRoutingTable(std::map<int, std::pair<int, int>> newTable){
    routingTable = newTable;
}

// Agrega una nueva conexión al enrutador si no existía anteriormente. Devuelve si la operación fue exitosa.
bool router::addConnection(std::pair<int, int> newConnection){
    bool completed = true;
    bool found = false;
    for(auto it = connections.begin(); it != connections.end(); ++it){
        if(it->first == newConnection.first){ // Si no nos estamos conectando a un nuevo enrutador
            found  = true;
            completed = false;
        }
    }
    if(!found) connections.push_back(newConnection);
    return completed;
}

//Busca el par que se eliminará y, si se encuentra, lo elimina. Devuelve si la operación fue exitosa.
bool router::deleteConnection(std::pair<int, int> conToDelete){
    bool completed = false;
    for(auto it = connections.begin(); it != connections.end(); ){
        if(it->first == conToDelete.first && it->second == conToDelete.second){
            it = connections.erase(it);
            completed = true;
        } else {
            it++;
        }
    }
    return completed;
}

/* Modifica la conexión si se encuentra. Devuelve si la operación fue exitosa.
El parámetro de entrada es un par con (nodo conectado para modificar la conexión con el nuevo peso).
*/
bool router::modifyConnection(std::pair <int, int> conToMod){
// Tenga en cuenta que no verifica si el nuevo peso es realmente diferente
    bool completed = 0;
    for(auto it = connections.begin(); it != connections.end(); ++it){
        if(it->first == conToMod.first){ // If the connection actually exists
            it->second = conToMod.second;
            completed = 1;
        }
    }
    return completed;
}

// Comprueba si el enrutador está conectado a otro enrutador dado.
bool router::isConnectedTo(int containsID){
    for(auto it = connections.begin(); it != connections.end(); ++it){
        if(it->first == containsID) return true;
    }
    return false;
}


// Imprime informacion sobre el enrutador
void router::print(){
    std::cout << "Router " << ID << ":\nConnections (Connected to, Weight):\n";
    for(std::pair p : connections){
        std::cout << '\t' << p.first << " , " << p.second << "\n";
    }
    std::cout << "Routing table (Destination router, Distance, Via which node):\n";
    for(auto const& [key, value] : routingTable){
        std::cout << '\t' << key << " , " << value.first << ", " << value.second << '\n';
    }
    std::cout << '\n';
}
