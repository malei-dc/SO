Archivos necesarios para crear la VM para los talleres 0 a 4 de SO.

Una vez instalado vagrant y sus prerequisitos:
vagrant up (para crear la vm y aprovisionarla de todo lo necesario)
vagrant ssh (para loguearse con usuario vagrant, que puede hacer sudo su)

La clave de root es SoyRoot.
Para loguearse como root: sudo su

Existen los usuarios taller0 a taller4. 
La clave es la misma que el usuario.
Los archivos necesarios para cada taller estan dentro del home del usuario correspondiente.
Por ejemplo, para loguearse como taller0: su - taller0

Si la configuración de teclado de la VM difiere de la de tu teclado, 
desde cualquier usuario podes cambiar la distribución usando según corresponda:
sudo loadkeys us
sudo loadkeys latam
sudo loadkeys es
