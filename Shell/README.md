# Shell
## Para ejecutar el proyecto :
```bash
gcc -o Shell Shell.c
./Shell
```

# GENERALIDADES:
Nuestro shell cumple con todos los puntos establecidos de la orden. De forma general:
1.	Se corre ciclo infinito que solo cesa cuando se introduce el comando exit.
2.	Sale en pantalla my-promp $ 
3.	Recibe la entrada del usuario y lo descompone en tokens
4.	Guarda la entrada en el history, cambiando en caso que se haya introducido el comando again.
5.	Comprueba si se ejecuta en el foreground o el background. En el primer caso espera a que el comando ejecute. En el caso del segundo se corre en un proceso hijo, del cual se guarda en su pid. Si luego se ejecuta el comando Jobs se listan los procesos que ocurren en el background. Se puede enviar un proceso en el background al foreground usando fg.
6.	Comprueba que si es el comando if y ejecuta sus componentes recursivamente.
7.	Comprueba los operadores ; , && y ||, llamando a sus operandos recursivamente.
8.	Comprueba si se ejecuta |, llamando a sus operadores recursivamente.
9.	Comprueba las redirecciones.
10.	Si el comando es uno propio de nuestro Shell lo ejecuta. Si no, llama execvp al comando.

# INICIO Y LECTURA DE ENTRADA
El Shell empieza en el archivo Shell.c, donde está presente el ciclo infinito. Este llama a la función readline, luego a SplitLine, ubicados en readline.h. readline lo que hace es tomar caracteres hasta que se entre EOF o \n. Si el usuario entra un espacio al principio o seguido de otro espacio lo ignora. Si el usuario entra # lo ignora todo a partir de ese punto. Si el usuario introduce un operador(; , ||, | , &&, & , etc) sin poner un espacio antes o después, el programa añade un espacio antes o después de añadir el operador. En cualquier otro caso, el carácter introducido es añadido. La funcion splitline se basa en la funcion strtok para a partir de la entrada separarla por espacios.

# ALL_EXECUTE Y HISTORIAL
Una vez que tenemos los tokens de entrada llamamos a all_Execute en el archivo exec.h . Este tiene la única funcion de guardar la entrada en el historial antes de ejecutar Execute. Si esto se llevara a cabo en Execute directamente, hubiera el problema de los llamados recursivos de este.
Para implementar el history se utilizo de manera auxiliar un archivo denominado history_file_path.txt. Este archivo puede contener a lo sumo 10 lineas, correspondientes a los 10 ultimos comandos ejecutados. Si el archivo del historial no existe, este se crea. Para esto nos apoyamos en la lectura/escritura de ficheros.Cada vez que se escribe un comando a ejecutar en la consola, este se agrega al history y si este tiene mas de 10 comandos, se quita el primero de estos y se corren los 9 ultimos hacia arriba, y finalmente se agrega; el ultimo comando entrado. Aunque se cierre el shell, al volverse a abrir se va a mantener el historial. Se implemento la funcionalidad again leyendo entre los comandos escritos en el historial y ejecutando de nuevo el comando correspondiente al indice indicado.

# EL BACKGROUND
Lo primero que se comprueba en la funcion Execute es si este proceso se llevara en el background o no. En ese caso, se llamara a la funcion background_Execute presente en el mismo archivo exec.h. Se hace uso de la struct background, que tiene un array con los pid de los procesos que están corriendo, otro array con sus nombres y el entero total que indica la cantidad de procesos. Por tanto, la funcion hace lo siguiente:
-Rearma la entrada (recordemos que esta tokenizada) usando la funcion Reconstruct y la guarda en la ultima posición del array.
-hace fork una primera vez.
-En el hijo se guarda su pid en el background para poder acceder a el de nuevo, luego hace fork y llama a Execute con el nieto. El hijo espera al nieto y una vez que este termina quita tanto el nombre de su proceso como el pid de su proceso de la struct.
-El padre termina sin esperar al hijo con el fin de continuar el programa, dejando que el hijo se encargue de ejecutar el background.
IF-THEN-ELSE-END
Lo segundo que comprueba Execute es si el comando es if, lo cual llama a la funcion CaseIf. La razón por la que se llama al if aquí es porque tiene menor prioridad que los operadores. Esta funcion lo que hace es:
-buscar las posiciones de Then, Else y End. Si encuentra uno o varios if ignora todo hasta que encuentre su o sus respectivos End.
-La posición del Then y la del End deben existir, y debe respetarse el orden.
-Usando la funcion Separate(la cual se explicara mas adelante) se obtiene lo que esta entre el if y el then, lo que esta entre el then y el else (o end en caso de que no haya else) y entre el else y el end(en caso de que exista el else); y se guardan esos tokens en un array.
-Se llama a Execute con la primera parte: si devuelve 1, entonces devuelve Execute con la segunda parte, sino, en caso de que haya un else, devuelve Execute con la 3ra parte.
&& ; || | < > >>
Volvemos a Execute. Si el ultimo token no es & y el primero no es if:
-Se recorren los tokens hasta encontrar ; , && o ||.
-Si encuentra un ; llama a Execute recursivamente con los tokens a la izquierda y luego retorna Execute con los tokens a la derecha.
-Si encuentra un && llama a Execute recursivamente con los tokens a la izquierda y si devuelve 1 retorna Execute con los tokens a la derecha. Sino devuelve 0.
-Si encuentra un || llama a Execute recursivamente con los tokens a la izquierda y si devuelve 0 retorna Execute con los tokens a la derecha. Sino devuelve 1.
-Si no se encuentran estos operadores, se vuelven a recorrer los tokens buscando un |, en este caso hace un pipe y en un primer hijo ejecuta los tokens de la izquierda cambiando la escritura estándar y en un segundo hijo ejecuta los tokens a la derecha cambiando la lectura estándar.
-Si no se encuentran pipes se vuelve a recorrer el array buscando <, > o >>, donde se ejecutan los tokens que les preceden cambiando la lectura o escritura estándar de acuerdo con el operador al archivo con el nombre del token que le sigue.
-Tener en cuenta que en todos estos operadores se usa la funcion Separate, que toma 4 entradas, y guarda en la cuarta dos arrays de tokens, los que están antes de la posición indicada por la tercera entrada del array de la primera entrada y los que están despues (en otras palabras, devuelve los conjuntos de lo que esta antes y despues de un token).

# VARIABLES
Para la implementacion de las variables se creo una struct VaraibleList que simula una lista de variable con su valor  correspondiente. Esta lista posee funciones que permiten agregar y eliminar variables.
La struct VariableList posee el campo cant que indica la cantidad de variables actualmente, un array names con los nombres  de las variables y un array values para los valores de las variables. Los metodos implementados son push_variable,que es utilizado para agregar variables y pop_variable para eliminarlas. Estas funciones son la logica detras de set y unset. Por otra parte, existe las funcion print_variables_values, que imprime todas las varibles con sus valores al escribir el comando set solo y la funcion print_varaible que imprime en consola el valor de una variable especifica, lo cual constituye la logica detras del get. Las variables y sus valores se mantienen solo durante la ejecucion del shell, es decir, al cerrar este se elimina el rastro de estas variables. En el caso de las variables cuyo valor resulta de evaluar un comando y coger su salida por consola, se crea un archivo auxiliar denominado output.txt, que contiene la salida de este comando que deberia salir en consola y es lo que se guarda luego en la variable.



# LOS COMANDOS 
Una vez que ya los tokens no tienen ningún tipo de operador, se comprueba si el primero es de los que mandaban en la orden a crear o de los ajenos. En el segundo caso despues de comprobar se llaman a la funcion execvp. En el primerO
