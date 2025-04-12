#include <iostream>
#include <vector>
#include <string>

using namespace std;

class itemPregunta{ 
private:
  int id;
  string pregunta;
  string tax;
  int tiempoEst;

public:
  itemPregunta(int id, string pregunta, string tax, int tiempoEst): id(id), pregunta(pregunta), tax(tax), tiempoEst(tiempoEst) {}
  ~itemPregunta() {}
// Geters
 int getID() { return id; }
 string getPregunta() { return pregunta; }  
 string getTax() { return tax; }
 int getTiempoEst() { return tiempoEst; }
// Seters
 void setId(int id) { this->id = id; }
 void setPregunta(string pregunta) { this->pregunta = pregunta; }
 void setTax(string tax) { this->tax = tax; }
 void setTiempoEst(int tiempoEst) { this->tiempoEst = tiempoEst; }

 void print(){
    cout << "ID: " << id << "Pregunta: " << pregunta << "Taxonomia: " << tax << "Tiempo Estimado: " <<   tiempoEst << endl;
  }
};

class menuCRUD{
private:
  vector<itemPregunta> preguntas;
public:
  menuCRUD() {}
  ~menuCRUD() {}

  void crearItem(itemPregunta item) { preguntas.push_back(item); }

  void borrarItem(int id) {
    for (auto it = preguntas.begin(); it != preguntas.end(); ++it) {
      if (it->getID() == id) {
        preguntas.erase(it);
        return;
      }
    }
    cout << "Item no encontrado " << endl;
    }

  void actualizarItem(int id, itemPregunta item){
    for (auto it = preguntas.begin(); it != preguntas.end(); ++it) {
      if (it->getID() == id) {
        *it = item;
        return;
      }
    }
    cout << "Item no encontrado " << endl;
  }


  void mostrarItems(){
    if (preguntas.empty()) {
      cout << "No hay preguntas " << endl;
      return;
    }
    for (auto &item : preguntas){
      item.print();
    }
  }

  int tiempoPrueba(){
    int prueba = 0;
    for (auto it = preguntas.begin(); it != preguntas.end(); ++it) {
      prueba += it->getTiempoEst();
    }
    return prueba;
  }

};


int main() { std::cout << "Hello World!\n"; }