#import "@preview/ilm:1.1.1": *

#set text(lang: "es")

#show: ilm.with(
  title: [Camarón 3D],
  author: "Manual de usuarios",
  date: datetime(year: 2024, month: 06, day: 08),
  abstract: [
    Version 1.0
  ],
  figure-index: (enabled: false),
  table-index: (enabled: false),
  listing-index: (enabled: false)
)

#let table_color(seq, color) = [
  #text(color)[#seq]
]

#let itemText(seq) = [
  #text(
    weight: "bold",
  )[#seq]
]

#let linkText(seq) = [
  #text(
    fill: blue,
    style: "oblique",
    weight: "thin",
  )[#underline[#seq]]
]

#let optionText(seq) = [
  #text(
    style: "oblique",
    weight: "thin"
  )[#seq]
]

= ¿Qué es Camaron3D? <Camaron>
Camarón 3D es una herramienta computacional enfocada en la visualización de mallas poligonales y mixtas. Tiene la capacidad de trabajar sobre distintos formatos para evaluar los elementos de la malla a tiempo real de diferentes evaluaciones u operaciones entregadas para el usuario.

Es un software que ha sido desarrollado por distintos exalumnos del Departamento de las Ciencias de la Computación de la Universidad de Chile como tema de memoria para optar al grado de Ingeniería Civil en Computación en dicha universidad.

Los inicios del software se remontan al año 2012 donde se desarrolló la primera versión de Camarón, posteriormente se han agregado nuevas funcionalidades, reingeniería y optimizaciones al código fuente.

La misión de Camarón 3D es ser un software rápido, optimizado y capaz de manejar modelos de mallas grandes para poder aplicar análisis sobre más de un millón de elementos.

= Funcionalidades Actuales <Func>
Camarón 3D posee diversas funcionalidades como:
- #itemText([Carga y exportación]): \Camarón tiene soporte a diversos archivos utilizados para almacenar dicha información en el software para ser manejada con mayor facilidad en las diversas funcionalidades existentes en Camarón.
- #itemText([Evaluación sobre la malla]): Actualmente, Camarón tiene diversas implementaciones que se pueden aplicar sobre mallas de polígonos y/o mallas de poliedros.
- #itemText([Visualización]): Existen diversas formas de visualización del modelo, las cuales poseen diversas funcionalidades y características que el usuario puede utilizar para observar y evaluar el modelo.
- #itemText([Métodos de selección]): Existen diversas estrategias de selección de los elementos de Camarón para poder identificar y seleccionar fácilmente los puntos de interés de la malla a su elección.
- #itemText([Estadísticas]): Se puede observar fácilmente estadísticas generales y estadísticas de los algoritmos de evaluación sobre la malla cargada.

= Instalación <Install>
== Windows
1. Descargar e instalar #link("http://qt-project.org/downloads")[#linkText[QTCreator, QTLibrary]] y el compilador de C++ #link("http://www.mingw.org/")[#linkText[MinGW]] (version 4.4 o mayor).

2. Descargar e instalar #link("http://glew.sourceforge.net/")[#linkText[GLEW]] (versión 1.9.0 o mayor)

3. Descargar e instalar #link("http://glm.g-truc.net/")[#linkText[GLM]] (versión 0.9.3.4 o mayor). Solo es necesario que se copie los archivos en la carpeta del proyecto.

4. Para compilar el software, es necesario recopilar la librería Mungw QT.

5. Configurar QTCreator para poder utilizar la librería QT y el compilador #link("http://mingw-w64.sourceforge.net/")[#linkText[MinGW-w64.]]

6. Compilar el proyecto Camarón utilizando QTCreator.

== Ubuntu
+ Descargar e instalar el software #link("https://www.qt.io/offline-installers")[#linkText[QTCreator]] y la libreria #link("https://doc.qt.io/qt-5/linux.html")[#linkText[QT]].

+ Descargar e instalar GLEW (version 1.9.0 o mayor).

+ Descargar e instalar GLM (version 0.9.3.4o mayor).

+ Compilar el proyecto Camarón utilizando QTCreator.
  - Debe abrir una terminal en el path del proyecto Camarón.
  - Debe ejecutar el comando qmake.
  - Al finalizar la ejecución del comando anterior, debe ejecutar el comando make.
  - Con ello se crearán los archivos necesarios y el ejecutable llamado "Camarón".
  
= Importación y exportación de mallas <CyEMalla>
En esa sección se explicitará las diversas funcionalidades que poseen Camarón sobre la carga y exportación de modelos de mallas poligonales o poliédricas.

#set align(center)
#table(
  columns: 3,
  [#text(weight: "bold")[Formato]], [#text(weight: "bold")[Importación]], [#text(weight: "bold")[Exportación]],

  [.off], [#table_color([SI], green)], [#table_color([SI], green)], 
  [.visf], [#table_color([SI], green)], [#table_color([SI], green)], 
  [.ply], [#table_color([SI], green)], [#table_color([NO], red)], 
  [ele/node], [#table_color([SI], green)], [#table_color([SI], green)], 
  [.tri], [#table_color([SI], green)], [#table_color([NO], red)], 
)
#set align(start)

== Cofiguración principal <MainConfig>
Camarón posee un menú enfocado a los parámetros globales que se utilizarán para el uso de la aplicación. Estos parámetros pueden ser modificados por el usuario, que van desde los opciones globales de la renderización, propiedades de OpenGL y la fucnionalidad de dibujos de la identificaciones de cada elemento.

=== ¿Cómo acceder?
Para llegar al menú de configuraciones principales, debe:

+ Dar clic en la opción #optionText[Configurations] en la barra de herramientas de Camarón.

+ En el menú desplegado, debe dar clic en #optionText[Main Configuration].

+ Con ello se arbirá el siguiente menú

=== Renderización
La primera pestaña de la configuración principal contiene parámetros generales para la renderización del modelo, como por ejemplo:
+ Color de fondo en donde se dibujará el modelo.
+ Proyección que desea utilizar.
+ Frame Rate Display: Pequeño recuadro que indica la cantidad de fotogramas por segundos con losque se dibuja la malla.
 - Se puede habilitar o no el dibujo de dicho recuadro.
 - Se puede cambiar el tamaño y color de la fuente, además del color del rectángulo que la contiene.

// imagen

=== Propiedades de OpenGL
La segunda pestaña contiene algunos parámetros que se pueden modificar sobre OpenGL, como:
+ Face culling: se puede habilitar y cambiar la orientación a su preferencia.
+ Las propiedades de las primitivas:
  - Line: Cambia el largo de los llamados de OpenGl para dibujar lineas, como lo hace #optionText[Normal Renderer].
  - Point: Cambia el tamaño de los llamados para dibujar puntos, como lo realizado por #optionText[Vertex Renderer].
  - Además, se puede cambiar la preferencia para dibujar con o sin antialiasing.
+ Bleending: Se encarga de habilitar el canal de transparencia para las renderizaciones al momento de dibujarlas para observarlas facilmente utilizando las renderizaciones secundarias (para dibujar varias renderizaciones a la vez).
  - Existe la opción para habilitar o no el canal de transparencia.
  - #itemText[Alpha Test]: Indica cuales renderizaciones no se sobrepondran sobre otras.
  - #itemText[Weighted Strategy]: Indica si desea usar la estrategia de pesos para dibujar las renderizaciones. Cada una de las renderizaciónes tiene un peso asociado que será vital cuanta transparencia se tendrá cada una.
    - Existe 3 opciones: #optionText[None, Weighted Sum, Weighted Average].

=== Identificaciones
La tercera pestaña contiene los valores asociadoal dibujo de los valores de las identificaciones de los elementos del modelo.
+ Se puede cambiar los colores del texto y del relleno de las letras. Además de modificar el tamaño de la fuente.
+ Se puede indicar a que elementos se les va a dibujar su identificación. Existen cuarto opciones:
  - #itemText[Surface]: solo considera la superficie de la malla.
  - #itemText[Interior]: solo considera los elementos en el interior de la malla.
  - #itemText[Complete]: considera todos los elementos.
  - #itemText[Selected]: considera únicamente los elementos seleccionado.
+ Se puede indicar si desea usar la #optionText[CPU] o la #optionText[Aceleración de GPU].
+ Se puede indicar la cantidad de identificaciones que va a dibujar, junto con su estrategia:
  - #optionText[Smallest depht values].
  - #optionText[Smallesst distance to screen center].

== Carga de archivos <CargaMalla>
Para poder cargar un modelo a Camarón se debe: 
+ Dirigir a la barra de herramientas ubicada en la parte superior de Camarón y hacer clic sobre el campo #optionText[File] para poder mostrar un menú desplegable. 
+ Debe dar clic en la opción #optionText[Open...] en el menú desplegable.

#figure(
  image("images/loadModel.png"),
)

+ Posteriormente, se abrirá el explorador de archivo del sistema operativo utilizado para poder ubicar el archivo deseado con el modelo. 
+ Luego debe hacer clic en #optionText[Open].

Además, Camarón guardará la ubicación de los últimos 5 archivos utilizados para su expedita carga del modelo, para ello:

+ Dirigir a la barra de herramientas ubicada en la parte superior de Camarón y hacer clic sobre el campo #optionText[File] para poder mostrar un menú desplegable. 
+ Debe ubicar el puntero del ratón sobre la opción #optionText[Recent files].
+ Se le mostrará un nuevo menú desplegable donde se mostrarán los últimos archivos utilizados. Debe dar clic en su opción de preferencia para que empiece la carga del modelo.

#figure(
  image("images/loadModelRecent.png"),
)

Al momento de empezar la carga del archivo, se abrirá una ventana que mostrará el proceso de carga del modelo.

 Además, si desea eliminar el historial de archivos cargados en Camarón debe dar clic en la opción #optionText[Clear files] ubicada en el menú desplegable que muestra los últimos archivos utilizados.

== Exportación del modelo cargado <ExportMalla> 
Para poder exportar un modelo cargado en Camarón debe:
+ Dirigir a la barra de herramientas ubicada en la parte superior de Camarón y hacer clic sobre el campo "File" para poder mostrar un menú desplegable. 
+ Debe ubicar el puntero del ratón sobre la opción #optionText[Export as...] para abrir otro menú desplegable.
#figure(
  image("images/exportModel.png"),
)
+ El nuevo menú desplegable tendrá dos opciones.
  - #optionText[Complete Model As] para poder exportar la totalidad de la malla.
  - #optionText[Selection As] para poder exportar únicamente los elementos seleccionados por algún método de selección de elementos.

Al seleccionar una de las dos opciones descritas anteriormente, se abrirá una ventana emergente del explorador de archivos del sistema operativo utilizado, en el cual deberá buscar la carpeta de preferencia. Luego debe escribir el nuevo nombre del archivo junto con la extensión de preferencia (en caso de que escriba un formato no indicado en la tabla anterior, se le indicará el error con una ventana emergente). Luego debe apretar #optionText[Save].

== Cerrar el modelo cargado
En caso de cerrar el archivo del modelo cargado debe:
+ Dirigir a la barra de herramientas ubicada en la parte superior de Camarón y hacer clic sobre el campo #optionText[File] para poder mostrar un menú desplegable. 
+ Debe dar clic a la opción #optionText[Close Model] ubicada al final del menú desplegado.
#figure(
  image("images/closeModel.png"),
)

Con ello se borrarán los datos cargados sobre el modelo y todas las acciones realizadas por el usuario.

= Evaluación sobre mallas <Evaluate>
Camarón posee la característica de evaluar rápidamente los elementos que componen la malla, como sus vértices, polígonos y poliedros.

Actualmente están implementadas las siguientes evaluaciones:
- #text(weight: "bold")[Polígono:]
  - Área.
  - Mínimo ángulo interno.
  - Mínimo ángulo dihedral.
  - Máximo ángulo interno.
  - Máximo ángulo dihedral.
  - Aspect ratio.
  - Cantidad de vértices por polígono.
  - Bhatia y Lawrence TM
  - Radius ratio.
  
- #text(weight: "bold")[Poliedro:]
  - Superficie.
  - Volumen.
  - Mínimo ángulo sólido.
  - Máximo ángulo sólido.
  - Cantidad de caras por poliedro.
  
- #text(weight: "bold")[Tetraedro:]
  - Radius ratio.

== Evaluar malla
Para poder evaluar una malla de polígonos/poliedros, se deben seguir las siguientes indicaciones:

+ Después de cargar el modelo en Camarón, se debe dirigir a la ventana de #optionText[Evaluation] ubicada a la izquierda de la ventana principal. En caso de que no esté visible, debe habilitarla dando clic en la opción #optionText[Evaluation Strategy] ubicada en la opción #optionText[Windows] en la barra de herramientas. #figure(
  image("images/Eval/eval0.png"),
)

+ La ventana contendrá un listado de opciones referentes a los algoritmos de evaluación disponibles, como se muestra en la siguiente figura. #figure(
  image("images/Eval/evalMenu.png"),
)

+ En ella debe seleccionar la estrategia de evaluación deseada y dar clic en #optionText[Evaluate].

== Visualizar el resultado <VisEval>
Existen diversas maneras de poder visualizar los resultados de los algoritmos de evaluación como:
- Utilizando las estadíscticas de Camaron en @Statics.
- Utilizando el render de propiedades en @PropertyRender
- Utilizando la estrategia de selección por propiedades en @SelectProperty.

Para entender como usar cada una de estas, haga clic sobre el nombre entre comillas del listado anterior para redireccionarlo a la sección correspondiente.

= Estadísticas de la malla <Statics>
Se encarga de dar a conocer al usuario información relevante obtenida sobre la malla. Existen dos tipos de estadísticas.
1. Model General Statics.
2. Element Statics.

== Model General Statics <MGStatics>
Se encarga de mostrar la información general de la malla cargada, por ejemplo, la cantidad de elementos encontrados o el tamaño de su Axis-Aligned Bounding Boxes.

=== ¿Cómo abrir la ventana?
Para poder abrir la ventana de la componente, debe:
+ Dar clic en la opción #optionText[Windows] en la barra de herramientas.

+ En el menú desplegable, debe posicionar el puntero del mouse sobre la opción #optionText[Statics].

+ En ella se abrirá un nuevo menú desplegable, donde deberá dar clic a #optionText[Model General Statics]. #figure(
  image("images/Statics/staticsToGeneral.png", width: 70%),
)

+ Con ello se abrirá la siguiente ventana que posee toda la información general sobre el modelo. #figure(
  image("images/Statics/staticsGeneral.png", width: 50%),
)

== Elements Statics <EStatics> 
Entrega información obtenida gracias a ejecutar algún algoritmo de evaluación como el máximo y mínimo valor encontrado, la cantidad de elementos a evaluar y la cantidad evaluada. Además, posee un gráfico para mostrar visualmente la distribución de los valores en dicha evaluación, con un pequeño formulario para editarlo a gusto del usuario.

=== ¿Cómo acceder?
Para poder encontrar dicha funcionalidad, debe:
+ Dar clic en la opción #optionText[Windows] en la barra de herramientas.

+ En el menú desplegable, debe posicionar el puntero del ratón sobre la opción #optionText[Statics].

+ En ella se abrirá un nuevo menú desplegable, donde deberá dar clic en #optionText[Elements Statics]. #figure(
  image("images/Statics/staticsToElement.png", width: 60%),
)

+ Con ello se abrirá una nueva ventana emergente que posee lo descrito al comienzo de la sección. #figure(
  image("images/Statics/staticsElement.png", width: 60%),
)

=== ¿Cómo visualizar los valores de una evaluación?
Para poder visualizar gráficamente los valores calculados por alguna estrategia de evaluación, debe:
+ En un comienzo se debe haber ejecutado la estrategia de evaluación; en caso de no haber realizado esta acción, se debe dirigir a la sección.

+ Luego de haber ejecutado la estrategia de evaluación solicitada, debe escoger la misma opción en el listado de opciones. #figure(
  image("images/Statics/staticsElementOption.png", width: 40%),
)

+ Con ello, debe dar clic en #optionText[Refresh] para que se actualice el gráfico. #figure(
  image("images/Statics/staticsElementResult.png", width: 45%),
)

En caso de que el número de elementos evaluados sea igual a 0, es porque evaluó la malla con la misma opción seleccionada en #optionText[Element Statics]. Para poder solucionar dicho inconveniente solo debe seleccionar otra estrategia de evaluación para posteriormente volver a seleccionar la estrategia de evaluación deseada.

=== Cambiar el rango de valores para la visualización
Si desea cambiar la cantidad de columnas, grupos o rango de valores que se consideran para dibujar el grafico debe:

+ Abrir la ventana de #optionText[Elements Statics].

+ Debe cambiar los valores de #optionText[from]/ #optionText[to]/ #optionText[steps] a su preferencia
  - #optionText[from] es el menor valor a considerar en el gráfico.
  - #optionText[to] es el mayor valor calculado por la estrategia de evaluación.
  - #optionText[steps] es la cantidad de grupos o subconjuntos que considerará para gráficar, es decir, el número de columnas. #figure(
  image("images/Statics/staticsElementForm.png", width: 50%),
)

+ Para poder actualizar el gráfico con sus preferencias, debe hacer clic en #optionText[Refresh]

=== Conocer la cantidad de valor en cada columna
Para conocer la cantidad de valores encontrados entre los rangos de una columna debe:

+ Abrir la ventana de #optionText[Elements Statics] y tener ya evaluado el modelo con la estrategia de evaluación que desea visualizar.

+ Debe apretar #optionText[Refresh] para actualizar los valores del gráfico.

+ Debe colocar el puntero del ratón encima de la columna que quiere revisar. Con ello se mostrará un pequeño rectangulo con la cantidad de elementos entre esos rangos y el porcentaje con respecto al total de los elementos del modelo. #figure(
  image("images/Statics/staticsElementPointer.png", width: 50%),
)

=== Cambiar color de las columnas
Para conocer la cantidad de valores encontrados entre los rangos de una columna debe:

+ Abrir la ventana de #optionText[Elements Statics] y tener ya evaluado el modelo con la estrategia de evaluación que desea visualizar.

+ Debe apretar #optionText[Refresh] para actualizar los valores del gráfico.

+ Haga clic encima de cualquier columan del gráfico.

+ Se mostrará una ventana con diversas opciones de colores, en ella debe seleccionar su color de preferencia y apretar #optionText[Select].

= Rendering <Rendering>
Camarón tiene la funcionalidad de poseer varios tipos de renderizaciones que entregan diverso tipo de información para que el usuario pueda evaluar el modelo.

== ¿Cómo selecionar una renderización?
Para poder seleccionar otra renderización, debe:

+ En primer lugar debe haber cargado el modelo, ya que dependiendo del tipo del modelo se habilitarán o no.

+ Debe ubicar la ventana de #optionText[Renderer] ubicada al lado derecho de la ventana principal. En caso de que no se encuentre habilitada debe:
  - Abrir el menú desplegable #optionText[Windows] encontrado en la barra de herramientas y hacer clic en la opción de #optionText[Renderer] mostrado en el menú desplegable de #optionText[Windows]. Con ello se abrirá una ventana manejable en la ubicación dicha anteriormente. #figure(
  image("images/Renderer/renderOpen.png", width: 50%),
)

+ Se mostrará la siguiente ventana. #figure(
  image("images/Renderer/renderMenu.png", width: 40%),
)

+ Debe apretar el listado de opciones indicado en la imagen anterior para desplegar todas las opciones disponibles para el modelo cargado. #figure(
  image("images/Renderer/renderOptions.png", width: 40%),
)

+ En ella debe buscar la de su preferencia y hacer clic en ella.

== Abrir las opciones de configuración del renderización seleccionado
Para poder modificar variables de la renderización selecionada debe:

+ Debe estar disponible la ventana de #optionText[Renderer].

+ Debe seleccionar la renderización que desea modificar.

+ Debe dar clic en #optionText[Config] ubicado al costado del listado de opciones de renderizaciones disponibles. #figure(
  image("images/Renderer/renderConfig.png", width: 40%),
)

+ Con ello se abrirá una ventana emergente con formularios para editar la renderización seleccionada. Cada renderización tendrá su propio formulario de edición de dibujo, por lo que pueden o no compartir ciertas variables. Para saber qué aspectos se pueden modificar, puede ir a la sección de tipos de renderizaciones donde se hablará más sobre ello y lo que contiene cada formulario de configuración.

== renderización secundario
=== ¿Cómo agregar un renderización secundario?
Una renderización secundaria es una forma de dibujar de varias formas el modelo cargado para visualizar simultáneamente distintas renderizaciones a la vez. Para ello debe:

+ Seleccionar la renderización que desee que sea secundario.

+ Dar clic en botón #optionText[+] ubicado al costado. #figure(
  image("images/Renderer/renderAddSecond.png", width: 40%),
)

+ Con ello, Camarón empezará a dibujar el modelo con la renderización seleccionada, además puede selecionar más de una renderización secundaria a la vez, que podra visualizar en la ventana #optionText[Secondary Renderer]. #figure(
  image("images/Renderer/renderSecond.png", width: 45%),
)

=== ¿Cómo eliminar un renderización secundario?
Si desea sacar una renderización secundaria, debe:

+ Ubicar el recuadro con el listado de renderizaciones secundarias seleccionados.

+ Debe hacer clic encima de la renderización que desea eliminar.

+ Debe apretar la tecla suprimir.

Con ello se dejará de considerar dicha renderización como habilitada y se borrará del listado de #optionText[Secondary Renderers].

== Tipos de renderizaciones
=== Phong Shading
Es una técnica de interpolación que permite obtener el sombreado (intensidad de color) de las superficies en gráficos 3D. Camarón posee dos renderizaciones de este tipo: una utilizando los píxeles dibujados y otra utilizando los vértices del modelo.

#figure(
  image("images/Renderer/phongRenderer.png", width: 60%)
)

#text(weight: "bold")[Configuración]\
Los dos tipos de renderizado de Phong comparten el formulario de configuración. En ella se puede modificar las siguientes características:
- #itemText[Inverse Back Face Color] cambia el color de la parte interna del polígono al inverso del seleccionado para el color del material.
- #itemText[Translucent mode] habilita un modo de transparencia en la malla cargada.
- Se puede cambiar los valores de #itemText[Material] y #itemText[Light] de la técnica de Phong, junto a su #itemText[Shininess] (valores altos concentra más la luz reflejada) y la posición desde donde se encuentra la luz reflejada en el material.
- En #itemText[Draw elements] existen tres opciones que cambia totalmente los elementos dibujados de la malla cargada
  - *Complete* dibujará la totalidad de la malla
  - *Selected* dibujará solamente los elementos seleccionados de la malla por la estrategia de evaluación utilizada.
  - *Unselected*  dibujará solamente los elementos *no* seleccionados de la malla por la estrategia de evaluación utilizada.

#figure(
  image("images/Renderer/phongConfig.png", width: 30%),
)

=== Flat color and wireframe
Este tipo de renderizado dibuja las caras de la malla (los polígonos) de un color uniforme y las aristas de un color llamativo para así identificar fácilmente los elementos de la malla.


#figure(image("images/Renderer/mainRender.png", width: 60%))

#text(weight: "bold")[Configuración]\
En esta renderización se puede modificar los siguientes valores:
- Se puede habilitar o no si se quiere dibujar los #itemText[Wireframes]. Además, se puede cambiar el color de dicho elemento.
- Se puede modificar el color base con que se dibuja el modelo, ya sean los elementos seleccionados como los no seleccionados (inicialmente estos serán blancos y amarillos).
- Los elementos que se dibujarán. Existen 5 opciones:
  - *Surface* dibujará los elementos perteneciente a la superficie de la malla.
  - *Interior* dibujará los elementos que no pertenecen a la superficie de la malla. Esta opción se visualizará mejor si el modelo cargado es una malla de poliedros.
  - *Unselected* dibujará solamente los elementos de la malla que se encuentren no seleccionados por la estrategia de selección seleccionada.
  - *Selecetd* dibujará solamente los elementos de la malla que se encuentren seleccionados por la estrategia de selección seleccionada.
  - *Complete* dibujará la totalidad de los elementos de la malla cargada.

#figure(image("images/Renderer/mainConfig.png", width: 40%))


=== Polyhedron Kernel
Esta renderización no dibuja la figura cargada por el usuario, sino que muestra el kernel de la malla cargada. Para saber más sobre esta renderización, puede ir a la sección @Kernel.

=== Vectores normales
Este renderización dibujará vectores que representa las normales de cada vértice que contenga la malla cargada.

#figure(image("images/Renderer/normalRender.png", width: 60%))

#text(weight: "bold")[Configuración]\
Se pueden modificar dos aspectos principales de este renderización
- Se puede modificar los valores de colores del vector #itemText[normal], tanto para la cola (el inicio del vector, punto en la superficie) como la punta, además de la longitud.
- Se puede habilitar o no los #itemText[Wireframes] de la malla.
  - Se puede escoger el color con el que se dibujaran.
  - Se puede escoger que se dibujará (Ninguno, solamente los de la superficie o todos).

#figure(image("images/Renderer/normalConfig.png", width: 40%))

=== Propiedades <PropertyRender>
Dibujará de una manera similar a #optionText[Flat color and wireframes] con la especialidad de destacar los elementos del modelo que cumplan algún tipo de condición dada a través de un formulario y los valores calculados por alguna estrategia de evaluación

#figure(image("images/Renderer/propertyRender.png", width: 60%))

#text(weight: "bold")[Configuración]\
En la ventana de configuración se puede ingresar diversas referencias como:
+ #itemText[Evaluation Strategy]: Elegir la estrategia de evaluación que será utilizada para dibujar la malla.
+ #itemText[Wireframes]: Si desea dibujar o no las aristas de la malla, además de poder modificar el color de la misma, si algún polígono que pertenece se encuentra seleccionado por alguna estrategia de selección.
+ #itemText[Face Color Range]: En esta opción se modifica los colores que se utilizarán para colorear los elementos de la malla con respecto a su valor en la estrategia de evaluación seleccionada.
  - #itemText[No value]: Color utilizado para los elementos con valor nulo.
  - #itemText[Min value]: Color utilizado para los elementos con valores inferiores en comparación al resto.
  - #itemText[Max value]: Color utilizado para los elementos con valores superiores en comparación al resto.

#figure(image("images/Renderer/propertyConfig.png", width: 40%))

Los colores ingresados en #itemText[Face Color Range] se utilizarán para realizar una degradación de colores entre todos los valores obtenidos por la estrategia de evaluación en los elementos de la malla.

Además, existe la opción #optionText[Draw only selected elements] que dibujará solamente los elementos seleccionados por alguna estrategia de selección.

=== Glass Renderer
Dibujará la malla cargada simulando estar hecha de vidrio. Capaz de mostrar más información de otras renderizaciones como #optionText[Polyhedron Kernel Renderer] o ver el interior de una malla de poliedros.

#figure(image("images/Renderer/glassRenderer.png", width: 60%))

Se pueden modificar dos aspectos principales en esta renderización, separados por un menú con pestañas.
- Se puede modificar los valores de la técnica de Phong utilizada.
- Se puede indicar qué elementos  de la malla dibujar (seleccionados, no seleccionados o la totalidad) con la capacidad de cambiar el color.

#figure(image("images/Renderer/glassConfig.png", width: 50%))

== Geometría de intersección <GeometryRender>
Tiene la característica de cambiar el color con el que se dibuje la figura dependiendo de que se encuentre dentro de una geometría que la intersecte. Actualmente, existen dos tipos de geometría disponibles: la esfera y los planos.

#figure(image("images/Renderer/geometryRenderer.png", width: 60%))

#text(weight: "bold")[Configuración]\
Para la configuración de la renderización existen dos formularios que se distinguen por la pestaña en la que se encuentre el usuario. Existe una configuración de la renderización #itemText[Rendering Config] y una para modificar la geometría de intersección #itemText[Geometry].

#figure(image("images/Renderer/convexConfig1.png", width: 50%))

En la configuración de la renderización se puede modificar los siguientes parámetros.
- #itemText[ Cut Border Color]: El color de los elementos que se encuentren en el borde de la intersección.
- Se puede modificar el color base y seleccionado de los elementos que se encuentren al interior de la geometría, además de habilitar o no si se quiere dibujar únicamente dichos elementos.
- Se puede modificar el color y tamaño de los #optionText[wireframes].
- Se puede cambiar el color base de los elementos seleccionados y no seleccionados.
- Se puede indicar que elementos de la figura se quiere dibujar.
  - *Surface* dibujará los elementos perteneciente a la superficie de la malla.
  - *Interior* dibujará los elementos que no pertenecen a la superficie de la malla. Esta opción se visualizará mejor si el modelo cargado es una malla de poliedros.
  - *Unselected* dibujará solamente los elementos de la malla que se encuentren no seleccionados por la estrategia de selección seleccionada.
  - *Selecetd* dibujará solamente los elementos de la malla que se encuentren seleccionados por la estrategia de selección seleccionada.
  - *Complete* dibujará la totalidad de los elementos de la malla cargada.
  
En la configuración de la geometría se tienen dos aspectos: las propiedades de la intersección y la geometría convexa. En el siguiente párrafo se hablará únicamente de los valores de las propiedades de la intersección. En caso de que requiera información sobre la geometría convexa para la intersección, la puede encontrar en las subsecciones disponibles.

#figure(image("images/Renderer/convexConfig2.png", width: 60%))

Para la intersección se puede modificar los siguientes parámetros:
- El #itemText[color] de la geometría convexa que se dibujará (inicialmente es un color verde con su valor de transparencia igual a 0.1).
- Se puede modificar el color y largo del vector #itemText[normal], y sí que requiere que se dibuje o no.
- Se puede invertir la categorización de los elementos que indica si están seleccionados o no (los seleccionados pasarían a ser no seleccionados y viceversa).
- Se puede indicar si la figura sigue al modelo o no.

=== Esfera
Esta figura representará a una esfera que contendrá los siguientes valores.
- #itemText[Center]: Posición de la esfera.
- #itemText[Radius]: Radio de la esfera.
- #itemText[N° Vertices]: El número de vértices que se utilizará para la discretización de la esfera

#figure(image("images/Renderer/convexConfig4.png", width: 50%))

Actualmente solo se puede tener una esfera a la vez dibujada.

Ya cuando haya ingresado los valores deseados, debe apretar "Apply" para aplicar los cambios.

También se puede modificar los valores explicitados anteriormente mediante inputs del usuario a través del teclado y ratón. Si quiere conocer más sobre esta funcionalidad puede leer la sección @Controlador

=== Plano
Inicialmente la cantidad de planos dibujada será 0, si desea agregar un plano debe apretar el botón llamado #optionText[Add Plane]. Con ello se incluirá un nuevo formulario que contendrá los siguientes parámetros:

- #itemText[Normal]: que representa el vector normal para sus tres ejes del plano.
- #itemText[Point]: que representa la posición donde estará ubicada la normal del plano.

Si desea agregar más planos, puede volver a dar clic en #optionText[Add Plane]. Existiendo un límite de 5 planos a la vez.

En caso de querer eliminar un plano creado, solamente debe apretar el botón #optionText[Delete] ubicado en la parte inferior de la sección de dicho plano.

#figure(image("images/Renderer/convexConfig3.png", width: 50%))

Ya cuando haya ingresado los valores deseados, debe apretar #optionText[Apply] para aplicar los cambios.

También se pueden modificar los valores explicitados anteriormente mediante inputs del usuario a través del teclado y ratón. Si quiere conocer más sobre esta funcionalidad, puede leer la sección @Controlador.

== Vertex Renderer
Mostrará unicamente los vértices de la malla cargada de forma similar a lo que sería una nube de puntos.

#figure(image("images/Renderer/vertexRender.png", width: 60%))

#text(weight: "bold")[Configuración]\
Se puede modificar los valores de colores RGBA de los vértices dibujados.

#figure(image("images/Renderer/vertexConfig.png", width: 40%))

== Coloreo de altura
Colorea la superficie de la malla cargada dependiendo de su distancia con respecto a un eje. Inicialmente, realizará este coloreo tomando en consideración el eje Z. Se recomienda ir directamente a las configuraciones para ingresar los valores del #optionText[AABB (Axis Aling Bounding Box)] de la malla para obtener una mejor visualización.

#figure(image("images/Renderer/heightRenderer.png", width: 60%))

#text(weight: "bold")[Configuración]\
Existen 3 aspectos principales para modificar al utilizar este renderización:
+ #itemText[Coloring Bounding]: Se puede modificar los valores mínimos y máxmios para colorear la superficie de la malla. Se recomienda apretar el boton "Set bounds from Model" para cambiar estos valores al #[AABB] de la malla cargada.
+ #itemText[Color Mode]: Puede intercambiar los colores utilizados entre uno multicolor (Hue scale) o una escala de grises (Gray scale).
+ #itemText[Height axis]: Puede cambiar el eje de referencia utilizado para dibujar la figura.

#figure(image("images/Renderer/heightConfig.png", width: 50%))

Además, esta la opción de intercambiar la escala de colores utilizadas para valores altos y bajosAdemás, esta la opción de intercambiar la escala utilizada. marcando el checkbox #itemText[Inverse intensity].

== Cartesian Axis
Dibuja el #optionText[Axis Aling Bounding Box (AABB)] de la malla.

#figure(image("images/Renderer/aabbRenderer.png", width: 60%))

#text(weight: "bold")[Configuración]\
Existen cuatro opciones que el usuario puede cambiar, en todas ellas se puede cambiar el color y el grosor/tamaño.
- El #itemText[contorno] del AABB.
- El punto #itemText[origen] del espacio.
- Los #itemText[ejes].
- Los #itemText[vertices] del AABB.

#figure(image("images/Renderer/aabbConfig.png", width: 70%))

= Selección de elementos
Camarón tiene una funcionalidad capaz de identificar los elementos de interés del usuario. Existen diversas maneras/estrategias que el usuario puede usar para distinguir unos elementos de otros.

Para apreciar bien los elementos seleccionados por las estrategias de selección, se recomienda utilizar la renderización #optionText[Flat colors, wireframes].

=== Selección utilizando el ratón.
Utiliza el ratón para poder seleccionar los elementos. La forma de utilizar esta selección se muestra en la sección de Controladores.

Al arrastrar el ratón, se creará un rectángulo donde se seleccionarán todos los elementos que estén dentro de él. Además, al seleccionar dicha estrategia, muestra el siguiente formulario para ayudar al usuario a seleccionar elementos de la malla.

#figure(image("images/Selection/selectMouse.png", width: 50%))

Hay una sección con 3 opciones (Vertex, Polygon y Polyhedron). La opción seleccionada indicará que elementos se buscarán utilizando esta estrategia.

Una sección con los siguientes tipos de selección: 
- #itemText[New clean selection] realiza una selección desde cero.
- #itemText[Join with current selecition] realiza una unión de conjuntos de los elementos ya catalogados como seleccionados y los clasificados como seleccionados con esta estrategia de evaluación.
- #itemText[Intersect with current] realiza una intersección de conjuntos de los elementos ya catalogados como seleccionados y los clasificados como seleccionados con esta estrategia de evaluación.
- #itemText[Invert] invierte la categorización de los elementos seleccionados a no seleccionados y viceversa.

Una última sección que indica las condiciones de los elementos para ser considerados seleccionados:
- #itemText[Surfaces]:  los elementos que pertenecen a la superficie de la malla.
- #itemText[Interior]: los elementos que no pertenecen a la superficie de la malla.
- #itemText[All]: considera tanto a los elementos de la superficie como el interior.

=== Selección mediante sus propiedades. <SelectProperty>
Esta estrategia de evaluación identificará los elementos de la malla que se haya cargado dependiendo de su valor está entre unos rangos establecidos por el usuario en la estrategia de evaluación dada.

#figure(image("images/Selection/selectProperty.png", width: 50%))

La imagen anterior hace referencia a la ventana de Estrategias de Selección cuando se escoge la opción "Select by Property". En ella se aprecia un listado de opciones referente a las estrategias de evaluación, más un formulario para indicar el valor mínimo y máximo a considerar como elemento seleccionado.

Existen diversos tipos de selección:
- #itemText[Apply over Model]: revisa los valores de todos los elementos evaluados por la estrategia de evaluación seleccionada.
- #itemText[Apply over Model]: revisa los valores de únicamente los valores ya catalogados como seleccionados, revisando los valores dados por la estrategia de evaluación seleccionada.

=== Selección mediante las ids de los elementos.
Con este método se pueden seleccionar los elementos de una malla con respecto a su valor dado como identificador único.

#figure(image("images/Selection/selectId.png", width: 50%))

Para poder utilizar esta estrategia se deben ingresar dos valores, el mínimo y el máximo de los valores de los identificadores que serán seleccionados. Además existen 3 métodos de selección:
- #itemText[New clean selection]: realiza una selección desde cero.
- #itemText[Join with current selecition]: realiza una unión de conjuntos de los elementos ya catalogados como seleccionados y los clasificados como seleccionados con esta estrategia de evaluación.
- #itemText[Intersect with current]: realiza una intersección de conjuntos de los elementos ya catalogados como seleccionados y los clasificados como seleccionados con esta estrategia de evaluación.

Al final de la ventana, hay una sección con 3 opciones (#optionText[Vertex, Polygon y Polyhedron]). La opción seleccionada indicará qué elementos se buscarán utilizando esta estrategia de evaluación por el identificador.

== Selección mediante la geometría de selección
Esta estrategia tiene relación con la renderización "Geometry Intersection". Para saber más sobre dicho render puede ir a la sección @GeometryRender.

#figure(image("images/Selection/selectGeometry.png", width: 50%))

Al momento de seleccionar dicha estrategia se mostrará la ventana anterior. En ella se pueden reconocer dos secciones importantes:
- Una sección con 3 opciones (#optionText[Vertex, Polygon y Polyhedron]) donde la opción seleccionada indicará que elementos se buscarán utilizando esta estrategia.
- Un listado de 4 opciones para indicar el tipo de selección que se realizara:
  - #itemText[New clean selection] realiza una selección desde cero.
  - #itemText[Join with current selecition] realiza una unión de conjuntos de los elementos ya catalogados como seleccionados y los clasificados como seleccionados con esta estrategia de evaluación.
  - #itemText[Intersect with current] realiza una intersección de conjuntos de los elementos ya catalogados como seleccionados y los clasificados como seleccionados con esta estrategia de evaluación.
  - #itemText[Invert] invierte la categorización de los elementos seleccionados a no seleccionados y viceversa.

== Expandir la selección
Existe una opción capaz de expandir la selección de los elementos seleccionados a los vecinos de dichos elementos. Para ello debe escojer la estrategia de selección #optionText[Expand Selection].

En dicha opción se abrirá la siguiente ventana.

#figure(image("images/Selection/selectExpand.png", width: 50%))

En ella se puede escojer tres opciones:
- #optionText[All]: Expande a los vecinos de todos los elementos seleccionados (tanto para los que están en la superficie como el interior de la figura, si se trata de una malla de poliedros).
- #optionText[Only Surface]: se expandirá unicamente a los vecinos de los elementos seleccionados que se encuentren en la superficie.
- #optionText[Only Surface with angle]: se expandirá a los elementos vecinos de los elementos seleccionados que tengan una angulo dihedral menor al valor ingresado. Dicho valor debe estar en radianes.

== Abrir la tabla de elementos seleccionados
Si desea conocer o tener más información sobre los elementos seleccionados, puedes revisarlos mediante la tabla de elementos seleccionados. Para llegar a ellas debes:

+ En la barra de herramientas de Camarón, debes dar clic a la opción #optionText[Windows].

+ En ella se desplegará un menú del cual debe dar clic a la opción #optionText[Selection].

+ Con ello se mostrarán dos opciones de las cuales deberá apretar #optionText[Selection Table].

#figure(image("images/Selection/selectionMenuTable.png", width: 60%))

Con ello se abrira la siguiente ventana.

#figure(image("images/Selection/selectionTable.png", width: 50%))

La primera sección indica el tipo de elementos que se consideran para la selección, ya sea vértices, polígonos o poliedros, con el número de elementos seleccionados.

En la segunda sección se muestra una tabla que indica las identificaciones de los elementos seleccionados y las propiedades de dichos elementos seleccionados, en caso de que existan o se hallan cargados al momento de importar un archivo.

= Controlador <Controlador>
Camarón posee soporte para dispositivos de teclado y ratón para ayudar al usuario a la visualización y manejo del modelo y las funcionalidades que actualmente posee. A continuación se listará las acciones que se puede realizar con Camarón.

=== Manejo del modelo principal
#table(
  columns: (auto, 1fr),
  inset: 10pt,
  align: horizon,
  table.header(
    [*Input*], [*Descripción*],
  ),
  "Key Up", "Cambia la posición del modelo aumentando una unidad sobre el eje Y",
  "Key Down", "Cambia la posición del modelo disminuyendo una unidad sobre el eje Y",
  "Key Left", "Cambia la posición del modelo disminuyendo una unidad sobre el eje X",
  "Key Right", "Cambia la posición del modelo aumentandom una unidad sobre el eje X",
  "Ctrl + Key Up", "Cambia la posición del modelo aumentandom una unidad sobre el eje Z",
  "Ctrl + Key Down", "Cambia la posición del modelo aumentandom una unidad sobre el eje Z",
  "Key Space", "Restea la posición, rotación y escala del modelo",
  "Key O", "Habilta la visualización de las identificaciones de los poliedros",
  "Key U", "Habilta la visualización de las identificaciones de los vertices",
  "Key H", "Cambia al modo de edición de la geometria de intersección. Para más información puede ir a la sección sobre Manejo de las geometrias de intersección en las subsecciones siguientes.",
  "Rueda mouse", "Cambia el valor de la escala con que se dibuja el modelo",
  "Botón izquierdo mouse", "Rota el modelo sobre el eje Z",
  "Botón derecho mouse", "Rota el modelo sobre el eje Y",
  "Ctrl + Botón izquierdo mouse", "Traslada el modelo en los ejes X e Y",
)

=== Manejo de las geometrias de intersección
Los siguientes inputs dependerán de varias condiciones:
- Se debe tener seleccionado la renderización #optionText[Geometry Intersection]
- Se debe seleccionar una de las figuras disponibles para intersectar la malla.

Actualmente, existen dos tipos de geometrías que pueden ser modificadas mediante el teclado y el ratón. Estos inputs cambiarán dependiendo de la figura seleccionada.

==== Esfera
#table(
  columns: (auto, 1fr),
  inset: 10pt,
  align: horizon,
  table.header(
    [*Input*], [*Descripción*],
  ),
  "Key H", "Entra al modo de edición de geometria de intersección de Esfera.",
  "Key Space", "Restea la posición, rotación y escala del modelo.",
  "Key Q", "Al mantener apretado la tecla, habilita la edición de la geometría en el eje XY.",
  "Key A", "Al mantener apretado la tecla, habilita la edición de la geometría en el eje YZ.",
  "Key Z", "Al mantener apretado la tecla, habilita la edición de la geometría en el eje XZ.",
  "Rueda mouse", "Cambia el valor de la escala con que se dibuja el modelo.",
  "Botón izquierdo mouse", "Rota el modelo sobre el eje Z.",
  "Botón derecho mouse", "Rota el modelo sobre el eje Y.",
  "Ctrl + Botón izquierdo mouse", "Traslada el modelo en los ejes X e Y.",
  "Key Q + Botón izquierdo mouse", "Cambia la posición de la esfera en el eje XY.",
  "Key A + Botón izquierdo mouse", "Cambia la posición de la esfera en el eje YZ.",
  "Key Z + Botón izquierdo mouse", "Cambia la posición de la esfera en el eje XZ.",  
  "Key W + Botón izquierdo mouse", "Al arrastrar, aumenta o disminuye el radio de la esfera dependiendo si el puntero del mouse se acerca o aleja del centro de la esfera.",  
)

==== Planos
#table(
  columns: (auto, 1fr),
  inset: 10pt,
  align: horizon,
  table.header(
    [*Input*], [*Descripción*],
  ),
  "Key H", "Entra al modo de edición de geometria de intersección de Planos.",
  "Key Space", "Restea la posición, rotación y escala del modelo.",
  "Key Q", "Al mantener apretado la tecla, habilita la edición de la geometría en el eje XY.",
  "Key A", "Al mantener apretado la tecla, habilita la edición de la geometría en el eje YZ.",
  "Key Z", "Al mantener apretado la tecla, habilita la edición de la geometría en el eje XZ.",
  "Key E", "Cambia el plano a modificar, habilitando la edición del (i+1)-esimo plano creado.",
  "Rueda mouse", "Cambia el valor de la escala con que se dibuja el modelo.",
  "Botón izquierdo mouse", "Rota el modelo sobre el eje Z.",
  "Botón derecho mouse", "Rota el modelo sobre el eje Y.",
  "Ctrl + Botón izquierdo mouse", "Traslada el modelo en los ejes X e Y.",
  "Key Q + Botón izquierdo mouse", "Cambia la posición de la esfera en el eje XY.",
  "Key A + Botón izquierdo mouse", "Cambia la posición de la esfera en el eje YZ.",
  "Key Z + Botón izquierdo mouse", "Cambia la posición de la esfera en el eje XZ.",
  "Key Q + Rueda mouse", "Rota la normal de plano en su propio eje cambiando los valores de en el eje XY.",
  "Key A + Rueda mouse", "Cambia la posición de la esfera en el eje YZ.",
  "Key Z + Rueda mouse", "Cambia la posición de la esfera en el eje XZ.",  
)

= Calculo del kernel del poliedro <Kernel>
Esta componente de Camarón difiere de la estrategia de evaluación descrita anteriormente, además de que se deben realizar pasos extras en comparación a los indicados en la sección de Evaluación sobre mallas.

== Evaluar el modelo
Para poder evaluar el modelo en Camarón para obtener el kernel de la malla que representa se debe.
+ Cargar un modelo al software.
+ Debe dirigirse a la barra de herramientas de Camarón ubicada en la parte superior de la ventana para dar clic a la opción #optionText[Windows].
+ Se abrirá un menú desplegable con distintas opciones, en ellas debe dar clic a la opción #optionText[Polyhedron Kernel]. #figure(
  image("images/PolyKernel/polyKernelMenu.png"),
)
+ Se abrirá una nueva ventana emergente que muestra distinta información sobre el kernel del modelo.
  Inicialmente, se mostrará algo similar a la siguiente pantalla. #figure(
  image("images/PolyKernel/polyKernel0.png", width: 60%),
)
+ Para iniciar el proceso del cálculo del kernel, debe dar clic en #optionText[Calculate Kernel] ubicado en la parte inferior de la ventana emergente. Con ello se abrirá una nueva ventana que indica el progreso del proceso en el cálculo del kernel.
+ Cuando se complete el algoritmo, se cambiará el estado de la ventana desde un #optionText[Not Calculated] a #optionText[Calculated]. Además, se cambiará los valores de la cantidad de vértices y caras del kernel, que inicialmente estaban en 0. Si es que estos valores siguen siendo 0 quiere decir que no existe el kernel para dicha figura cargada.
#figure(
  image("images/PolyKernel/polyKernel1.png", width: 60%),
)

== Visualizar el kernel del modelo
Para poder visualizar el modelo resultante posterior a ejecutar el algoritmo se deben seguir los siguientes pasos

+ Para esta funcionalidad se debe ir a la ventana de #optionText[Rendering].
+ Debe escoger la opción de #optionText[Polyhedron Kernel Renderer] del listado de opciones de #optionText[Renderer] (Si es que se encuentra disponible). #figure(
  image("images/PolyKernel/polyKernelRender.png", height: 40%),
)
+ Al seleccionar automaticamente dibujará la figura.

== Exportar el kernel del modelo
Para poder exportar el modelo resultante a un formato de archivo soportado posterior a ejecutar el algoritmo, se deben seguir los siguientes pasos:

+ Debe dirigirse a la barra de herramientas de Camarón ubicada en la parte superior de la ventana para dar clic a la opción "Windows".
+ Se abrirá un menú desplegable con distintas opciones, en ellas debe dar clic a la opción #optionText[Polyhedron Kernel].
+ Se abrirá una nueva ventana emergente que muestra distinta información sobre el kernel del modelo. #figure(
  image("images/PolyKernel/polyKernelExport.png", width: 60%),
)
+ Debe dar clic al botón "Export Model". Dicho botón puede estar deshabilitado si es que no se ha ejecutado el cálculo del kernel. Para poder habilitarlo, debe dar clic en #optionText[Calculate Kernel] para evaluar el modelo.

== Modificar colores 
Para poder modificar los colores con los que se dibujará el kernel de la malla debe:
+ Seleccionar #optionText[Polyhedron Kernel Renderer].
+ Dar clic al botón #optionText[Config] al costado del listado de opciones de #optionText[Renderer], con ello abrirá la siguiente ventana emergente. #figure(
  image("images/PolyKernel/polyKernelConfig.png"),
)
+ En ella, puede ingresar los valores de los colores de las aristas y de las caras de la malla.
+ Luego, debe apretar #optionText[Apply] para aplicar los cambios al modelo.

= Bugs y errores reportados
En la siguiente sección se listarán todos los errores reportados, indicando su gravedad.

#table(
  columns: (auto, auto, 1fr),
  inset: 10pt,
  align: horizon,
  table.header(
    [*Componente*], [*Nivel*], [*Descripción*]
  ),
  [Carga de modelos],  [#table_color([Leve], yellow)], [No se cierra la ventana de dialogo sobre carga de modelo con archivos livianos.],
  [Carga de modelos], [#table_color([Medio], orange)], [Algunos archivos .visf no son identificados correctamente. Aún no se sabe el origen.],
  [Calculo Kernel],  [#table_color([Grave], red)], [Camaron3D se cierra repentinamente al tratar de calcular el kernel de la malla con archivos cargados en el formato ele/node. Es porque no considera correctamente los elementos de la superficie.],
  [Selección],  [#table_color([Grave], red)], [Camaron3D se cierra repentinamente al aplicar la estrategia de expanción cuando ya hay elementos seleccionados.],
  [Selección], [#table_color([Medio], orange)], [La estrategía mediante mouse no selecciona los elementos de la malla.],
  [Renderización], [#table_color([Medio], orange)], [No funciona correctamente Depth Peeling.],
)