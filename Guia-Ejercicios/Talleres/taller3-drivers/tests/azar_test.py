#!/usr/bin/env python3
import unittest
import os

class nuloTestCase(unittest.TestCase):
    def test_azar(self):
        print("== Verificando escritura y lectura en /dev/azar ==")
        print("* Abriendo dispositivo")
        dev = os.open("/dev/azar", os.O_RDWR)
        print("* Configurando limite en 256")

        os.write(dev, b'256')
        print("* Output en 16 iteraciones: ")
        for i in range(0,16):
            # El driver me devuelve en bytes, lo convierto a string y le saco el
            # ultimo caracter de \n
            rand_bytes = os.read(dev, 5).decode("utf-8")[0:-1]
            rand = int(rand_bytes)
            print(rand)
            assert(0 <= rand and rand < 256)
        
        # Para limpiar el dispositivo
        os.write(dev, b'0')
        
        print("* Cerrando dispositivo")
        os.close(dev)
    
    def test_leer_sin_escribir_falla(self):
        # Abro tres veces el dispositivo
        print("== Verificando lectura sola debe fallar en /dev/azar ==")

        print("* Abriendo dispositivo")
        dev = os.open("/dev/azar", os.O_RDWR)

        print("* Intentando leer debe fallar")
        with self.assertRaises(OSError):
            os.read(dev, 5)

        # Cierro lo que abrí
        os.close(dev)
    
    def test_escribir_nada_falla(self):
        # Abro tres veces el dispositivo
        print("== Verificando escritura no numerica debe fallar en /dev/azar ==")

        print("* Abriendo dispositivo")
        dev = os.open("/dev/azar", os.O_RDWR)

        print("* Intentando escribir un valor que no sea numerico debe fallar")
        with self.assertRaises(OSError):
            os.write(dev, b'fdf')

        # Cierro lo que abrí
        os.close(dev)
 

if __name__ == '__main__':
    unittest.main()
