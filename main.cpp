#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Item {
protected:
    int id;
    string texto;
    string nivel;
    string tipo;
    int tiempo;
    string respuesta;

public:
    Item(int i, string txt, string n, string t, int temp, string resp)
        : id(i), texto(txt), nivel(n), tipo(t), tiempo(temp), respuesta(resp) {}

    virtual void mostrar() const {
        cout << "\n[ID: " << id << "]\nPregunta: " << texto
             << "\nNivel taxonomico: " << nivel
             << "\nTipo: " << tipo
             << "\nTiempo estimado: " << tiempo << " min"
             << "\nRespuesta correcta: " << respuesta << endl << "\n";
    }

    // Getters
    int getId() const { return id; }
    string getNivel() const { return nivel; }
    int getTiempo() const { return tiempo; }


    virtual void actualizar() {
        cout << "\n === Actualizar pregunta ID: " << id << " ===\n";
        cout << "Nuevo enunciado: ";
        getline(cin >> ws, texto);
        cout << "Nueva respuesta correcta: ";
        getline(cin >> ws, respuesta);
    }

    virtual ~Item() {}
};

class ItemOpcionMultiple : public Item {
private:
    string a, b, c, d;

public:
    // Constructor para preguntas de opción múltiple
    ItemOpcionMultiple(int i, string txt, string n, int temp, string rpta,
                       string altA, string altB, string altC, string altD)
        : Item(i, txt, n, "Opcion Multiple", temp, rpta), a(altA), b(altB), c(altC), d(altD) {}

    // Muestra la pregunta con sus alternativas y la respuesta correcta
    void mostrar() const override {
        cout << "\n[ID: " << id << "]\nPregunta: " << texto
             << "\nNivel taxonomico: " << nivel
             << "\nTipo: " << tipo
             << "\nTiempo estimado: " << tiempo << " min"
             << "\nAlternativas:"
             << "\na. " << a << "\nb. " << b << "\nc. " << c << "\nd. " << d << ""
             << "\nRespuesta correcta: " << respuesta << endl << "\n";
    }

    // Permite actualizar enunciado, alternativas y respuesta correcta
    void actualizar() override {
        cout << "=== Actualizar pregunta ID: " << id << " ===\n";
        mostrar();
        cout << "Nuevo enunciado: ";
        getline(cin >> ws, texto);
        cout << "Nueva alternativa a: "; getline(cin >> ws, a);
        cout << "Nueva alternativa b: "; getline(cin, b);
        cout << "Nueva alternativa c: "; getline(cin, c);
        cout << "Nueva alternativa d: "; getline(cin, d);
        cout << "Nueva respuesta correcta (a, b, c, d): "; getline(cin, respuesta);
    }
};

// Asigna el tiempo estimado en minutos según el nivel taxonómico
// Basado en la complejidad cognitiva de la Taxonomía de Bloom:
int tiempoPorNivel(int nivel) {
    switch(nivel) {
        case 0: return 1;  // 0 - Recordar: tareas simples de memoria => 1 min
        case 1: return 2;  // 1 - Entender: explicar/comprender => 2 min
        case 2: return 3;  // 2 - Aplicar: usar conocimiento => 3 min
        case 3: return 4;  // 3 - Analizar: distinguir/organizar => 4 min
        case 4: return 5;  // 4 - Evaluar: emitir juicios razonados => 5 min
        case 5: return 6;  // 5 - Crear: generar nuevo conocimiento => 6 min
        default: return 0;
    }
}

class Evaluacion {
private:
    vector<Item*> items;
    void pausarMenu() const {
        string pausa;
        cout << "\nPresione 1 y Enter para volver al menu... ";
        getline(cin >> ws, pausa);
    }

public:
    Evaluacion() {}

    ~Evaluacion() {
        for (Item* i : items) delete i;
    }

    // Crea uno o más items según la cantidad ingresada
    void crearItem() {
        int cantidad;
        cout << "\nCuantas preguntas quieres crear? ";
        cin >> cantidad;

        for (int i = 0; i < cantidad; ++i) {
            int id = items.size() + 1;
            string texto, respuesta, tipoNombre;
            int nivelInt, tipoInt;
            string nivelNombre;

            cout << "\n--- Ingresa los datos de la Pregunta " << i + 1 << " ---\n";
            cout << "Enunciado de la pregunta: "; getline(cin >> ws, texto);
            cout << "Nivel taxonomico (0: Recordar, 1: Entender, 2: Aplicar, 3: Analizar, 4: Evaluar, 5: Crear): ";
            cin >> nivelInt;
            switch(nivelInt) {
                case 0: nivelNombre = "Recordar"; break;
                case 1: nivelNombre = "Entender"; break;
                case 2: nivelNombre = "Aplicar"; break;
                case 3: nivelNombre = "Analizar"; break;
                case 4: nivelNombre = "Evaluar"; break;
                case 5: nivelNombre = "Crear"; break;
                default: nivelNombre = "Desconocido"; break;
            }
            int tiempo = tiempoPorNivel(nivelInt);

            cout << "Tipo de pregunta (1: Opcion Multiple, 2: Verdadero/Falso): ";
            cin >> tipoInt;

            if (tipoInt == 1) {
                string a, b, c, d;
                cout << "=== Alternativas ===\n";
                cout << "a. "; getline(cin >> ws, a);
                cout << "b. "; getline(cin, b);
                cout << "c. "; getline(cin, c);
                cout << "d. "; getline(cin, d);
                cout << "Alternativa correcta (a, b, c, d): ";
                getline(cin, respuesta);

                Item* nuevo = new ItemOpcionMultiple(id, texto, nivelNombre, tiempo, respuesta, a, b, c, d);
                items.push_back(nuevo);
            } else {
                cout << "Respuesta correcta (V: Verdadero, F: Falso): ";
                getline(cin >> ws, respuesta);
                Item* nuevo = new Item(id, texto, nivelNombre, "Verdadero/Falso", tiempo, respuesta);
                items.push_back(nuevo);
            }
        }
        pausarMenu();
    }

// Elimina un item de la evaluación según su ID
void eliminarItem(int id) {
    auto it = find_if(items.begin(), items.end(), [=](Item* i) {
        return i->getId() == id;
    });

    if (it != items.end()) {
        cout << "\n=== Pregunta a Eliminar ===\n";
        (*it)->mostrar();
        cout << "------------------------\n";
        string confirmacion;
        cout << "Desea eliminar esta pregunta? (S: Si, N: No): ";
        cin >> confirmacion;

        if (confirmacion == "S" || confirmacion == "s") {
            delete *it;
            items.erase(it);
            cout << "\nItem eliminado correctamente.\n";
        } else {
            cout << "\nOperacion cancelada. El item no fue eliminado.\n";
        }
    } else {
        cout << "\nItem no encontrado.\n";
    }
    pausarMenu();
}

// Permite modificar una pregunta existente por ID
    void actualizarItem(int id) {
        for (auto& item : items) {
            if (item->getId() == id) {
                item->actualizar();
                cout << "\n=== Pregunta " << id << " Actualizada ===";
                item->mostrar();
                cout << "------------------------\n";
                pausarMenu();
                return;
            }
        }
        cout << "\nItem no encontrado.\n";
        pausarMenu();
    }

// Muestra una pregunta específica según su ID
void consultarItem(int id) const {
    bool encontrado = false;
    for (const auto& item : items) {
        if (item->getId() == id) {
            cout << "\n=== Consulta de Item ===\n";
            item->mostrar();
            cout << "------------------------\n";
            encontrado = true;
            break;
        }
    }
    if (!encontrado) {
        cout << "\nItem no encontrado.\n";
        cout << "------------------------\n";
    }
    pausarMenu();
}

// Muestra todas las preguntas de un nivel taxonómico específico
    void buscarPorNivel(int nivelInt) const {
        string nivelNombre;
        switch(nivelInt) {
            case 0: nivelNombre = "Recordar"; break;
            case 1: nivelNombre = "Entender"; break;
            case 2: nivelNombre = "Aplicar"; break;
            case 3: nivelNombre = "Analizar"; break;
            case 4: nivelNombre = "Evaluar"; break;
            case 5: nivelNombre = "Crear"; break;
            default: nivelNombre = "Desconocido"; break;
        }
        bool encontrado = false;
        cout << "=== Preguntas del nivel: " << nivelNombre << " ===";
        for (const auto& item : items) {
            if (item->getNivel() == nivelNombre) {
                item->mostrar();
                cout << "------------------------";
                encontrado = true;
            }
        }
        if (!encontrado) cout << "No se encontraron items para ese nivel.";
        pausarMenu();
    }

    // Calcula el tiempo total estimado para responder todas las preguntas
    int calcularTiempoTotal() const {
        int total = 0;
        for (const auto& item : items) total += item->getTiempo();
        return total;
    }

    // Muestra todas las preguntas de la evaluación
    void mostrarEvaluacion() const {
        cout << "\n=== Evaluacion Completa ===\n";
        for (const auto& item : items) {
            item->mostrar();
            cout << "------------------------\n";
        }
        cout << "Tiempo total estimado: " << calcularTiempoTotal() << " minutos.\n";
        pausarMenu();
    }
};

// Menú principal para interactuar con la evaluación
void menu() {
    Evaluacion prueba;
    int opcion;
    do {
        cout << "\n========= MENU DE EVALUACION =========\n";
        cout << "1. Crear Item\n";
        cout << "2. Actualizar Item\n";
        cout << "3. Borrar Item\n";
        cout << "4. Consultar Item\n";
        cout << "5. Buscar por Nivel Taxonomico\n";
        cout << "6. Mostrar Evaluacion Completa\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                prueba.crearItem();
                break;
            case 2: {
                int id;
                cout << "\nIngrese el ID del item a actualizar: ";
                cin >> id;
                prueba.actualizarItem(id);
                break;
            }
            case 3: {
                int id;
                cout << "\nIngrese el ID del item a borrar: ";
                cin >> id;
                prueba.eliminarItem(id);
                break;
            }
            case 4: {
                int id;
                cout << "\nIngrese el ID del item a consultar: ";
                cin >> id;
                prueba.consultarItem(id);
                break;
            }
            case 5: {
                int nivelInt;
                cout << "\nIngrese el nivel taxonomico (0: Recordar, 1: Entender, 2: Aplicar, 3: Analizar, 4: Evaluar, 5: Crear): ";
                cin >> nivelInt;
                prueba.buscarPorNivel(nivelInt);
                break;
            }
            case 6:
                prueba.mostrarEvaluacion();
                break;
            case 0:
                cout << "\nSaliendo del programa...\n";
                break;
            default:
                cout << "\nOpcion no valida. Intenta nuevamente.\n";
        }
    } while(opcion != 0);
}

int main() {
    menu();
    return 0;
}