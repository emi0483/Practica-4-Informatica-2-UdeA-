#ifndef NETWORK_H
#define NETWORK_H

#include "router.h"
#include <list>
#include <algorithm>

class network
{
private:
    //Los enrutadores se almacenan en una lista. No hay dos enrutadores con la misma identificación.
    std::list<router> networkRouters;

public:
    network(); // Constructor
    bool addRouter(router); // Solo agrega un nuevo enrutador si su identificación no existía anteriormente. Devuelve si la operación fue exitosa.
    std::vector<int> listRouters(); // Devuelve un vector que contiene el ID de las rutas en la red.
    std::vector<std::pair<int, int>> getRouterConnections(int ID); // Devuelve las conexiones de un enrutador dada su identificación
    std::map<int, std::pair<int, int>> getRouterRoutRable(int ID); // Devuelve la tabla de enrutamiento del enrutador especificado


    void updateRoutingTables(); // Actualizar o crear la tabla de enrutamiento para cada enrutador
    void printRouters(); // Imprimir información sobre cada enrutador
    bool modifyEdge(int startNode, int finalNode, int newWeight); // Modificar la conexión entre dos enrutadores
    bool createEdge(int startNode, int finalNode, int newWeight); // Crear borde entre dos nodos
    bool deleteEdge(int startNode, int finalNode, int weight); // Eliminar conexión entre dos nodos

    bool deleteRouter(int deleteID); // Eliminar el enrutador de la red.


    // Crear tabla de enrutamiento para un enrutador en particular. Devuelve la tabla de enrutamiento.
    std::map<int, std::pair<int, int>> dijkstras(router startRouter);
};

#endif // NETWORK_H
