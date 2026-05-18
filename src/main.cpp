#include <iostream>
#include "Paciente.h"

using namespace std;

Paciente* ObtenerCabeza();
Paciente* InsertarPaciente(int id, const char* nombre, int edad, const char* sintomas, int prioridad, const char* estado);
Paciente* BuscarPacienteID(int id);
Paciente* BuscarPacienteNombre(const char* nombre);
bool ModificarPacienteID(int id, const char* nuevoNombre, int nuevaEdad, const char* nuevosSintomas, int nuevaPrioridad, const char* nuevoEstado);
bool EliminarPacienteID(int id);
void MostrarPacientes();
void GuardarPacientesEnArchivo(const char* ruta);
void CargarPacientesDesdeArchivo(const char* ruta);
void LiberarLista();
void CopiarCadena(char* destino, int tam, const char* origen);
int CompararCadenas(const char* a, const char* b);

void EncolarPrioridad(Paciente* paciente);
Paciente* Desencolar();
void MostrarCola();
bool ColaVacia();
bool EliminarDeColaPorId(int id);
void VaciarCola();

void PushCama(int numero);
bool PopCama(int& numero);
void MostrarCamas();
bool PilaVacia();
void LiberarPila();

// Limpia el buffer de entrada.
void LimpiarEntrada()
{
  cin.clear();
  cin.ignore(10000, '\n');
}

// Lee un entero dentro de un rango.
int LeerEnteroRango(const char* prompt, int min, int max)
{
  int valor;
  while (true)
  {
    cout << prompt;
    if (!(cin >> valor))
    {
      cout << "Entrada invalida." << endl;
      LimpiarEntrada();
      continue;
    }
    LimpiarEntrada();
    if (valor < min || valor > max)
    {
      cout << "Debe estar entre " << min << " y " << max << "." << endl;
      continue;
    }
    return valor;
  }
}

// Lee un entero con minimo.
int LeerEnteroMinimo(const char* prompt, int min)
{
  int valor;
  while (true)
  {
    cout << prompt;
    if (!(cin >> valor))
    {
      cout << "Entrada invalida." << endl;
      LimpiarEntrada();
      continue;
    }
    LimpiarEntrada();
    if (valor < min)
    {
      cout << "Debe ser mayor o igual a " << min << "." << endl;
      continue;
    }
    return valor;
  }
}

// Lee una cadena no vacia.
void LeerCadenaNoVacia(const char* prompt, char* buffer, int tam)
{
  while (true)
  {
    cout << prompt;
    cin.getline(buffer, tam);
    if (cin.fail())
    {
      cout << "Entrada demasiado larga." << endl;
      cin.clear();
      cin.ignore(10000, '\n');
      continue;
    }
    if (buffer[0] == '\0')
    {
      cout << "El campo no puede estar vacio." << endl;
      continue;
    }
    return;
  }
}

// Lee una respuesta s/n.
bool LeerSiNo(const char* prompt)
{
  char respuesta[10];
  while (true)
  {
    cout << prompt;
    cin.getline(respuesta, sizeof(respuesta));
    if (cin.fail())
    {
      cin.clear();
      cin.ignore(10000, '\n');
      continue;
    }
    if (respuesta[0] == 's' || respuesta[0] == 'S')
    {
      return true;
    }
    if (respuesta[0] == 'n' || respuesta[0] == 'N')
    {
      return false;
    }
    cout << "Responda con s o n." << endl;
  }
}

// Imprime un paciente en una linea.
void ImprimirPaciente(const Paciente* paciente)
{
  if (paciente == NULL)
  {
    return;
  }
  cout << "ID: " << paciente->id
     << ", Nombre: " << paciente->nombre
     << ", Edad: " << paciente->edad
     << ", Sintomas: " << paciente->sintomas
     << ", Prioridad: " << paciente->prioridad
     << ", Estado: " << paciente->estado << endl;
}

int main()
{
  // Cargar datos guardados.
  CargarPacientesDesdeArchivo("pacientes.txt");

  // Armar la cola con los pacientes en espera.
  Paciente* temp = ObtenerCabeza();
  while (temp != NULL)
  {
    if (CompararCadenas(temp->estado, "Esperando") == 0)
    {
      EncolarPrioridad(temp);
    }
    temp = temp->siguiente;
  }

  // Registrar camas disponibles al inicio.
  int camasIniciales = LeerEnteroMinimo("Cantidad inicial de camas disponibles: ", 0);
  for (int i = 1; i <= camasIniciales; i++)
  {
    PushCama(i);
  }

  int opcion = 0;
  // Menu principal.
  while (opcion != 9)
  {
    cout << "\n=== Sistema de Gestion de Emergencias ===" << endl;
    cout << "1. Registrar paciente" << endl;
    cout << "2. Mostrar pacientes" << endl;
    cout << "3. Buscar paciente" << endl;
    cout << "4. Modificar paciente" << endl;
    cout << "5. Eliminar paciente" << endl;
    cout << "6. Mostrar cola de prioridad" << endl;
    cout << "7. Atender paciente" << endl;
    cout << "8. Gestionar camas" << endl;
    cout << "9. Guardar y salir" << endl;

    opcion = LeerEnteroRango("Opcion: ", 1, 9);

    switch (opcion)
    {
      case 1:
      {
        int id = LeerEnteroMinimo("ID: ", 1);
        if (BuscarPacienteID(id) != NULL)
        {
          cout << "Ya existe un paciente con ese ID." << endl;
          break;
        }

        char nombre[50];
        char sintomas[100];
        LeerCadenaNoVacia("Nombre: ", nombre, sizeof(nombre));
        int edad = LeerEnteroMinimo("Edad: ", 0);
        LeerCadenaNoVacia("Sintomas: ", sintomas, sizeof(sintomas));
        int prioridad = LeerEnteroRango("Prioridad (1=Alta, 2=Media, 3=Baja): ", 1, 3);

        Paciente* nuevo = InsertarPaciente(id, nombre, edad, sintomas, prioridad, "Esperando");
        EncolarPrioridad(nuevo);
        cout << "Paciente registrado y en cola de prioridad." << endl;
        break;
      }
      case 2:
      {
        MostrarPacientes();
        break;
      }
      case 3:
      {
        int tipo = LeerEnteroRango("Buscar por 1=ID, 2=Nombre: ", 1, 2);
        Paciente* encontrado = NULL;
        if (tipo == 1)
        {
          int id = LeerEnteroMinimo("ID: ", 1);
          encontrado = BuscarPacienteID(id);
        }
        else
        {
          char nombre[50];
          LeerCadenaNoVacia("Nombre: ", nombre, sizeof(nombre));
          encontrado = BuscarPacienteNombre(nombre);
        }

        if (encontrado == NULL)
        {
          cout << "Paciente no encontrado." << endl;
        }
        else
        {
          ImprimirPaciente(encontrado);
        }
        break;
      }
      case 4:
      {
        int id = LeerEnteroMinimo("ID del paciente a modificar: ", 1);
        Paciente* paciente = BuscarPacienteID(id);
        if (paciente == NULL)
        {
          cout << "Paciente no encontrado." << endl;
          break;
        }

        char nuevoNombre[50] = "";
        char nuevosSintomas[100] = "";
        char nuevoEstado[20] = "";
        int nuevaEdad = -1;
        int nuevaPrioridad = -1;
        bool reordenarCola = false;

        if (LeerSiNo("Modificar nombre (s/n): "))
        {
          LeerCadenaNoVacia("Nuevo nombre: ", nuevoNombre, sizeof(nuevoNombre));
        }
        if (LeerSiNo("Modificar edad (s/n): "))
        {
          nuevaEdad = LeerEnteroMinimo("Nueva edad: ", 0);
        }
        if (LeerSiNo("Modificar sintomas (s/n): "))
        {
          LeerCadenaNoVacia("Nuevos sintomas: ", nuevosSintomas, sizeof(nuevosSintomas));
        }
        if (LeerSiNo("Modificar prioridad (s/n): "))
        {
          nuevaPrioridad = LeerEnteroRango("Nueva prioridad (1=Alta, 2=Media, 3=Baja): ", 1, 3);
          reordenarCola = true;
        }
        if (LeerSiNo("Modificar estado (s/n): "))
        {
          LeerCadenaNoVacia("Nuevo estado: ", nuevoEstado, sizeof(nuevoEstado));
          reordenarCola = true;
        }

        bool actualizado = ModificarPacienteID(
          id,
          nuevoNombre[0] ? nuevoNombre : NULL,
          nuevaEdad,
          nuevosSintomas[0] ? nuevosSintomas : NULL,
          nuevaPrioridad,
          nuevoEstado[0] ? nuevoEstado : NULL);

        if (actualizado)
        {
          cout << "Paciente actualizado." << endl;
          if (reordenarCola)
          {
            EliminarDeColaPorId(id);
            if (CompararCadenas(paciente->estado, "Esperando") == 0)
            {
              EncolarPrioridad(paciente);
            }
          }
        }
        else
        {
          cout << "No se pudo actualizar el paciente." << endl;
        }
        break;
      }
      case 5:
      {
        int id = LeerEnteroMinimo("ID del paciente a eliminar: ", 1);
        if (EliminarPacienteID(id))
        {
          EliminarDeColaPorId(id);
          cout << "Paciente eliminado." << endl;
        }
        else
        {
          cout << "Paciente no encontrado para eliminar." << endl;
        }
        break;
      }
      case 6:
      {
        MostrarCola();
        break;
      }
      case 7:
      {
        if (ColaVacia())
        {
          cout << "No hay pacientes en la cola." << endl;
          break;
        }

        Paciente* paciente = Desencolar();
        if (paciente == NULL)
        {
          cout << "No hay pacientes en la cola." << endl;
          break;
        }

        CopiarCadena(paciente->estado, (int)sizeof(paciente->estado), "En atencion");

        cout << "Paciente atendido:" << endl;
        ImprimirPaciente(paciente);

        if (LeerSiNo("Asignar cama ahora (s/n): "))
        {
          int numeroCama = 0;
          if (PopCama(numeroCama))
          {
            cout << "Cama asignada: " << numeroCama << endl;
          }
          else
          {
            cout << "No hay camas disponibles." << endl;
          }
        }
        break;
      }
      case 8:
      {
        int opcionCamas = 0;
        while (opcionCamas != 4)
        {
          cout << "\n--- Gestion de camas ---" << endl;
          cout << "1. Mostrar camas disponibles" << endl;
          cout << "2. Asignar cama" << endl;
          cout << "3. Liberar cama" << endl;
          cout << "4. Volver" << endl;
          opcionCamas = LeerEnteroRango("Opcion: ", 1, 4);

          switch (opcionCamas)
          {
            case 1:
              MostrarCamas();
              break;
            case 2:
            {
              int numeroCama = 0;
              if (PopCama(numeroCama))
              {
                cout << "Cama asignada: " << numeroCama << endl;
              }
              else
              {
                cout << "No hay camas disponibles." << endl;
              }
              break;
            }
            case 3:
            {
              int numero = LeerEnteroMinimo("Numero de cama a liberar: ", 1);
              PushCama(numero);
              cout << "Cama registrada." << endl;
              break;
            }
            default:
              break;
          }
        }
        break;
      }
      case 9:
      {
        GuardarPacientesEnArchivo("pacientes.txt");
        LiberarLista();
        VaciarCola();
        LiberarPila();
        cout << "Informacion guardada. Saliendo..." << endl;
        break;
      }
      default:
        break;
    }
  }

  return 0;
}