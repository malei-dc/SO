#!/usr/bin/env python3
import unittest
import os

class nuloTestCase(unittest.TestCase):
    def test_leer_nulo(self):
        print("== Verificando lectura /dev/nulo ==")

        print("* Abriendo dispositivo")
        dev = os.open("/dev/nulo", os.O_RDWR)

        print("* Intentando lectura")
        data = os.read(dev, 10)
        self.assertEqual(data, b'')

        print("* Cerrando lectura")
        os.close(dev)
    
    def test_write_nulo(self):
        print("== Verificando escritura en /dev/nulo ==")

        print("* Abriendo dispositivo")
        dev = os.open("/dev/nulo", os.O_RDWR)

        print("* Intentando escribir")
        data = os.write(dev, b'Hola!')
        self.assertEqual(data, 5)

        print("* Intentando escribir de nuevo")
        data = os.write(dev, b'Hola! Sistema Operativo')
        self.assertEqual(data, 23)

        print("* Intentando lectura")
        data = os.read(dev, 10)
        self.assertEqual(data, b'')

        print("* Cerrando lectura")
        os.close(dev)

if __name__ == '__main__':
    unittest.main()