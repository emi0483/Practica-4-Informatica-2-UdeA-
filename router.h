#ifndef ROUTER_H
#define ROUTER_H

#include <vector> //TODO: Should this #include be above the ifndef statement?
#include <iostream>
#include <map>
#include <utility>

class router
{
private:
    int ID;
    // Los bordes de este vértice están representados por un vector de pares (vertexThatIsConnectedTo, weightOfTheEdge)
    //El peso de un borde nunca debe ser 0 o menos
    std::vector<std::pair<int, int>> connections;
    std::map<int, std::pair<int, int>> routingTable;

public:
    // Los métodos que tienen un tipo de retorno de bool devuelven si la operación fue exitosa.

    router(int ID); // Constructor
    void setRoutingTable(std::map<int, std::pair<int, int>>); // Asignar nueva tabla de enrutamiento

    bool addConnection(std::pair<int, int>); // Agrega una nueva conexión al enrutador si no existía anteriormente.
    bool deleteConnection(std::pair <int, int>); //Busca el par que se eliminará y, si se encuentra, lo elimina.
    /*Modifica la conexión si se encuentra.
     El parámetro de entrada es un par con (nodo conectado para modificar la conexión con el nuevo peso).*/
    bool modifyConnection(std::pair <int, int>);
    bool isConnectedTo(int containsID); //Comprueba si el enrutador está conectado a otro enrutador

    void print(); //ID de impresión, tabla de enrutamiento y conexiones del enrutador

    int getID(){return ID;}
    std::vector<std::pair<int, int>> getConnections(){return connections;}
    std::map<int, std::pair<int, int>> getRoutingTable(){return routingTable;}
};

#endif // ROUTER_H
