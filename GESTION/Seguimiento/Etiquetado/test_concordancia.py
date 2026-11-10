"""Pruebas de concordancia_etiquetadores.py.  Ejecutar:  python test_concordancia.py
Los datos son SINTETICOS, solo para verificar el calculo."""
import random
import unittest

import concordancia_etiquetadores as c


class Pruebas(unittest.TestCase):
    def test_acuerdo_perfecto(self):
        a = [1, 2, 3, 4, 2, 3]
        r = c.calcular(a, a)
        self.assertEqual(r["exacta"], 1.0)
        self.assertEqual(r["pm1"], 1.0)
        self.assertAlmostEqual(r["kappa"], 1.0)

    def test_caso_calculado_a_mano(self):
        a = [1, 1, 2, 2, 3, 3, 4, 4]
        b = [1, 2, 2, 3, 3, 4, 4, 4]
        r = c.calcular(a, b)
        self.assertAlmostEqual(r["exacta"], 5 / 8)
        self.assertEqual(r["pm1"], 1.0)
        # 3 pares difieren en 1 etapa (peso 1/9): numerador = (3/8)/9
        num = 3 / 8 / 9
        pa = [2 / 8] * 4
        pb = [1 / 8, 2 / 8, 2 / 8, 3 / 8]
        den = sum(((i - j) ** 2 / 9) * pa[i] * pb[j] for i in range(4) for j in range(4))
        self.assertAlmostEqual(r["kappa"], 1 - num / den, places=9)

    def test_contra_implementacion_independiente_numpy(self):
        try:
            import numpy as np
        except ImportError:
            self.skipTest("numpy no instalado")
        rnd = random.Random(7)
        for _ in range(200):
            n = rnd.randint(5, 60)
            a = [rnd.randint(1, 4) for _ in range(n)]
            b = [min(4, max(1, x + rnd.choice([-1, 0, 0, 0, 1, 2]))) for x in a]
            O = np.zeros((4, 4))
            for x, y in zip(a, b):
                O[x - 1, y - 1] += 1
            W = np.array([[(i - j) ** 2 / 9 for j in range(4)] for i in range(4)])
            E = np.outer(O.sum(1), O.sum(0)) / n
            if (W * E).sum() == 0:
                self.assertIsNone(c.kappa_cuadratico(a, b))
                continue
            esperado = 1 - (W * O).sum() / (W * E).sum()
            self.assertAlmostEqual(c.kappa_cuadratico(a, b), esperado, places=9)

    def test_kappa_no_definido(self):
        self.assertIsNone(c.kappa_cuadratico([2, 2, 2], [2, 2, 2]))

    def test_etapas_texto(self):
        self.assertEqual(c.parse_etapa("e3", 2, "x"), 3)
        self.assertEqual(c.parse_etapa(" 4 ", 2, "x"), 4)
        with self.assertRaises(SystemExit):
            c.parse_etapa("E5", 2, "x")


if __name__ == "__main__":
    unittest.main(verbosity=2)
