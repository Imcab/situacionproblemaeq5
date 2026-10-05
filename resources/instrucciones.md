# Actividad integral 1:

¿Qué tengo que hacer?

Se nos provee un archivo mostrando el tráfico dentro de una empresa que tras ser infectada por un una botnet fue utilizada en un ataque DDoS. A través de diversas actividades integradoras procesarás y analizarás dicho archivo buscando resolver algunas preguntas clave:

    ¿Cuál fue el primer host infectado?
    ¿Cuál es la ip del bot master?
    ¿Contra qué dominio se realizó el DDoS attack?

Para comenzar, debemos entender el formato de los datos. La bitácora de actividad está en formato csv (valores separados por comas) y cuenta con el siguiente formato:
Fecha (dd-mm-aaaa), hora (hh:mm:ss), ip origen, puerto origen, dominio origen, ip destino, puerto destino, dominio destino.

Donde cada renglón representa un evento registrado en la bitácora (una petición de conexión entre un origen y un destino). El carácter – se utiliza para distinguir valores que no aplican (por ejemplo una ip no asociada a un dominio, o una conexión que no especifica un puerto o un nodo que aún no tiene asignada una dirección ip).

Lamentablemente, la bitácora provista fue incorrectamente generada por el administrador de la red, por lo que los datos parecen estar desordenados. En esta primera actividad nos concentraremos en ordenarlos a partir de la fecha y hora de cada evento registrado. Así mismo, generaremos una utilidad que nos permita filtrar eventos a partir de un rango dado.
Instrucciones

    Completa el código provisto por el profesor para generar un programa que:
        Lea el archivo bitácora provisto que le corresponde a tu equipo y almacena su información en un vector.
            Cada elemento del vector deberá ser una estructura (struct) de tipo event, conteniendo el conjunto de valores que conforman cada evento de la bitácora. La estructura ya contiene campos para cada uno de los atributos de un evento. Revisa el ejemplo de uso para usarla adecuadamente.
        Ordene el vector de forma ascendente a partir de la fecha y hora. Utiliza el método std::sort de la librería <algorithm>.
            Es importante sobrecargar el operador de comparación < para el correcto funcionamiento de std::sort. De esta forma podremos determinar cual es el orden relativo entre diferentes eventos.
            Dado que el ordenamiento debe realizarse a partir de la fecha y hora del evento, revisa la documentación de la librería <ctime>.
        Almacene el resultado del ordenamiento en un archivo con el mismo formato que la bitácora.
            Este archivo será usado en futuras actividades integrales, por lo que es fundamental que te asegures sea correcto.
            Considera también que el contenido del archivo necesitará ser leído y cargado en futuras actividades, por lo que diseña el código para la lectura/procesamiento del archivo de forma modular, de tal forma que puedas reutilizar secciones relevantes en futuras actividades.
        Finalmente, solicite al usuario una fecha (sólo día, mes y año) y despliegue todos los eventos a partir de dicha fecha.
            Utiliza el método std::find_if de la librería <algorithm> para encontrar la primera ocurrencia.
                Es importante definir un predicado que determine el criterio de búsqueda que usará la función. Para hacerlo puedes usar una expresión lambda, un objeto functor o una función con un parámetro "atado" usando la función boost::bind.

Para finalizar, realiza una investigación sobre los algoritmos de ordenamiento y búsqueda utilizados en la resolución de esta actividad, así como su complejidad (revisa la documentación correspondiente).

 