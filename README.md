# Calculadora Gráfica con Parser Personalizado

Proyecto de calculadora con interfaz gráfica usando **GTK+3**, **Flex** y **Bison**, que permite evaluar expresiones matemáticas complejas.

---

## 🔍 Descripción General

Esta aplicación permite a los usuarios ingresar expresiones matemáticas a través de una GUI amigable. Internamente, la expresión se analiza léxica y sintácticamente para producir un cálculo preciso.

Componentes principales:
- Analizador léxico con **Flex**.
- Analizador sintáctico con **Bison**.
- Interfaz de usuario con **GTK+3**.

---

## 🗂️ Estructura de Archivos

| Archivo                    | Descripción                        |
|:---------------------------|:-----------------------------------|
| `calc.h`                   | Declaraciones generales.           |
| `calc.l`                   | Definición de tokens (Flex).       |
| `calc.y`                   | Definición de gramática (Bison).   |
| `calc.tab.c`, `calc.tab.h` | Código generado por Bison.         |
| `calc.lex.c`               | Código generado por Flex.          |
| `calcfuncs.c`              | Operaciones matemáticas avanzadas. |
| `gui_calc.c`               | Lógica de la GUI con GTK.          |
| `library.c`                | Funciones auxiliares.              |
| `CMakeLists.txt`           | Archivo de configuración de CMake. |

---

## 📊 Funcionamiento

1. **Ingreso**: El usuario introduce una expresión.
2. **Tokenización**: Flex convierte la entrada en tokens.
3. **Parseo**: Bison analiza la estructura de la expresión.
4. **Evaluación**: Se realiza el cálculo.
5. **Visualización**: El resultado se muestra en pantalla.

---

## 🚀 Compilación y Ejecución

Requisitos:
- `gcc`
- `flex`
- `bison`
- `gtk+3`
- `cmake`

Pasos:

```bash
mkdir build
cd build
cmake ..
make
./calc
```

---

## 📅 Tecnologías Utilizadas

- **Lenguaje**: C (C99)
- **Análisis Léxico**: Flex
- **Análisis Sintáctico**: Bison
- **GUI**: GTK+3
- **Sistema de Build**: CMake

---

## 📊 Mejoras Futuras

- Soporte para funciones trigonométricas avanzadas.
- Historial de operaciones.
- Variables de usuario.
- Migración a GTK 4.
- Mejor manejo de errores de usuario.

---

## 📖 Créditos

**Autor**: (By Chris - Partum)  
**Fecha**: (26 de abril de 2025)

---

✨ Proyecto realizado para fines académicos y de aprendizaje de compiladores y GUI en C.
