#include "network.h"

network::network()
{

}



// Agrega un enrutador. Solo agrega un nuevo enrutador si su identificación no existía anteriormente. Devuelve si la operación fue exitosa.
bool network::addRouter(router newRouter){
    bool completed = false;

    int newID = newRouter.getID(); // Identificacion del nuevo router
    std::vector<int> listID = listRouters(); // Lista de identificacion de los routers

    auto iter = std::find(listID.begin(), listID.end(), newID);
    if(iter == listID.end()){ // Si la identificación del nuevo enrutador no se encuentra en la lista de identenificaciones ya existnetes
        networkRouters.push_back(newRouter);
        completed = true;
    }
    return completed;
}



// Devuelve un vector que contiene el la identiciacion de las rutas en la red
std::vector<int> network::listRouters(){
    std::vector<int> listID;
    for(auto it = networkRouters.begin(); it != networkRouters.end(); it++){
        listID.push_back(it->getID());
    }
    return listID;
}



// Devuelve las conexiones de un enrutador dada su identificación
std::vector<std::pair<int, int>> network::getRouterConnections(int ID){
    for(auto it = networkRouters.begin(); it != networkRouters.end(); ++it){
        if(it->getID() == ID) return it->getConnections();
    }
    //TODO: implementar un return predeterminado si no se encuentra la identificación
}

// Devuelve la tabla de enrutamiento de la ruta especificada
std::map<int, std::pair<int, int>> network::getRouterRoutRable(int ID){
    for(auto it = networkRouters.begin(); it != networkRouters.end(); ++it){
        if(it->getID() == ID) return it->getRoutingTable();
    }
    //TODO: implementar un return predeterminado si no se encuentra la identifiacion
}



// Actualizar o crear la tabla de enrutamiento para cada enrutador
void network::updateRoutingTables(){
    for(auto it = networkRouters.begin(); it != networkRouters.end(); ++it){
        it->setRoutingTable(dijkstras(*it));
    }
}



// Imprimer informacion sobre cada router
void network::printRouters(){
    for(auto it = networkRouters.begin(); it != networkRouters.end(); ++it){
        it->print();
    }
}



// Modifica la coneccion entre dos routers
bool network::modifyEdge(int startNode, int finalNode, int newWeight){
    bool cond1 = false;
    bool cond2 = false;
    for(auto it = networkRouters.begin(); it != networkRouters.end(); ++it){
        if(it->getID() == startNode){
            cond1 = it->modifyConnection(std::make_pair(finalNode, newWeight)); // Modificar la conexión del nodo de inicio
        }
        if(it->getID() == finalNode){
            cond2 = it->modifyConnection(std::make_pair(startNode, newWeight)); // Modificar la conexión del nodo final
        }
    }
    return (cond1 && cond2);
}



// Crear borde entre dos nodos
bool network::createEdge(int startNode, int finalNode, int newWeight){
    bool cond1 = false;
    bool cond2 = false;
    for(auto it = networkRouters.begin(); it != networkRouters.end(); ++it){
        if(it->getID() == startNode){
            cond1 = it->addConnection(std::make_pair(finalNode, newWeight)); // Añadir una conecxion al nodo de inicio  (router)
        }
        if(it->getID() == finalNode){
            cond2 = it->addConnection(std::make_pair(startNode, newWeight)); // añadir una conexion al nodo de llegada (router)
        }
    }
    return (cond1 && cond2);
}

// Borrar una conezcion entre dos nodos
bool network::deleteEdge(int startNode, int finalNode, int weight){
    bool cond1 = false;
    bool cond2 = false;
    for(auto it = networkRouters.begin(); it != networkRouters.end(); ++it){
        if(it->getID() == startNode){
            cond1 = it->deleteConnection(std::make_pair(finalNode, weight));
        }
        if(it->getID() == finalNode){
            cond2 = it->deleteConnection(std::make_pair(startNode, weight));
        }
    }
    return (cond1 && cond2);
}

// Eliminar el enrutador de la red.
bool network::deleteRouter(int deleteID){
    bool cond1 = false;
    bool cond2 = false;
    //Eliminar todas las conexiones del nodo
    std::vector<std::pair<int, int>> deleteConnections = getRouterConnections(deleteID);
    for(std::pair delCon : deleteConnections){ // Recall connections is structured as ( nodeConnected, weight )
        if( deleteEdge(deleteID, delCon.first, delCon.second) ) cond1 = true;
        else cond1 = false;
    }

    //Eliminar el enrutador de la red
    auto itDel = networkRouters.end();
    for(auto it = networkRouters.begin(); it != networkRouters.end(); ++it){
        if(it->getID() == deleteID){
            cond2 = true;
            itDel = it;
            break;
        }
    }
    if(itDel != networkRouters.end()) networkRouters.erase(itDel);
}


//Explanation: https://www.youtube.com/watch?v=GazC3A4OQTE
std::map<int, std::pair<int, int>> network::dijkstras(router startRouter){
    // List that contains: ( nodeID, distanceToNode, viaWhichNode )
    std::list<std::array<int, 3>> distTable;

    //Crear "mesa de distancia" vacía
    std::vector<int> listID = listRouters();
    for(auto it = listID.begin(); it != listID.end(); ++it){
        distTable.push_back({*it, -7, -7}); // -7 significa valor no valido del índice.
    }

    // Establezca la distancia del enrutador de origen a 0 y muévalo al comienzo de la lista
    auto posTable = distTable.end();
    for(auto itr = distTable.begin(); itr != distTable.end(); ++itr){
        if((*itr)[0] == startRouter.getID()){ // Si se encuentra la identificación del enrutar
           posTable = itr;
           (*itr)[1] = 0; // La distancia de un modo a si mismo es iguala cero
        }
    }
    distTable.splice(distTable.begin(), distTable, posTable);

    // To store the results of the algorithm
    // The key is the router id. The pair contains (leght of path to router, via which router)
    std::map<int, std::pair<int, int>> routingTable;

    //Ciclo
    while(!distTable.empty()){
        auto currentPos = distTable.begin(); // Se toma el primer elemento en disttable
        int currentID = (*currentPos)[0];

        //Analizar las conexiones del enrutador actual (CurrentId)
        std::vector<std::pair<int, int>> posCurrentConections = getRouterConnections(currentID);
        std::vector<std::pair<int, int>> currentConections;
        // No considere las conexiones con enrutadores que ya están analizados y dentro de la routingTable
            bool inRoutTable = false;
            for(auto [key, val] : routingTable){
                if(key == j.first) inRoutTable = true;
            }
            if(!inRoutTable) currentConections.push_back(j);
        }

        // iterar sobre las conexiones del enrutador actual
        for(std::pair conn : currentConections){
            auto modDistTable = distTable.end(); //elemento para modificar en disttable
            int evalID = conn.first; // Identificacion del enrutador que se analiza en disttable

            //Nueva distancia obtenida analizando el enrutador actual (CurrentID)
            // Es la longitud del camino hacia el enrutador actual (currentID)más el peso del enlace al enrutador(evalID)
            int newDist = ((*currentPos)[1]) + conn.second;

            int oldDist = 666; // Entonces no es un valor de basura. El código siempre debe cambiar este valor.
            for(auto itr = distTable.begin(); itr != distTable.end(); ++itr){
                if((*itr)[0] == evalID){
                    // La distancia antigua es la distancia que ya existía en la distTable para evalID
                    oldDist = (*itr)[1];
                    modDistTable = itr; // Puntero al elemento de distTable para modificar (eso representa evalID)
                }
            }

            if(newDist < oldDist || oldDist == -7){ //Si la nueva distancia que se encuentra es menor que la distancia que tuvimos
                (*modDistTable)[1] = newDist; // Nueva (mas corta) distancia encontrada
                (*modDistTable)[2] = currentID; // La ruta más corta se encontró a través del enrutador actual

                // Encuentre la posición del primer elemento con mayor distancia que newDist
                auto newPos = distTable.begin();
                while(newPos != distTable.end() && (*newPos)[1] <= newDist && (*newPos)[1] != -7){
                    ++newPos;
                }
                /*Si se encuentra, mueva el elemento modificado ModDisttable a una nueva posición:
                 Justo antes del primer elemento que está a mayor distancia */

                if(newPos != distTable.end()){
                    distTable.splice(newPos, distTable, modDistTable);
                }
            }
        }

        /* Después de iterar sobre las conexiones del enrutador actual (CurrentId),
         El enrutador está listo y lo agregamos a la tabla de enrutamiento */
        routingTable.insert({currentID, {(*currentPos)[1], (*currentPos)[2]}}); // enrutador, (lenghtToIT, viaWhichRouter)
        distTable.pop_front(); // Borrar el primer enrutador distTable (currentID)

    }

    return routingTable;
}

