# simpleINI3

the third iteration of my simple ini

| Feature                                       | simpleINI2                      | simpleINI3                       |
|-----------------------------------------------|-----------------------------------------------|------------------------------------------------|
| **Sections**                                  | No support for sections.                     | Supports sections like `[Database]`, `[Settings]`. |
| **Key-Value Pairs**                           | Basic key-value pairs, e.g., `a = 1`.        | Traditional key-value pairs in sections.      |
| **Expression Support**                       | No support for expressions.                  | Supports expressions like `\(a)` in values.    |
| **Inline Comments**                           | No support for inline comments.              | Supports inline comments with `;`.           |
| **Multiple Sections**                        | No support for multiple sections.            | Supports multiple sections.                  |
| **Automatic Type Checking**                   | No automatic type checking.                  | Basic type checking for standard INI types.  |
| **Readability and Complexity**                | Simpler, more straightforward code.          | More generic, handles standard INI structure. |
| **Use Case Focus**                            | Tailored for simpler INI structures.         | Generic parser for various INI file types.   |

**Note:** The D code is more suitable for simple INI structures with basic key-value pairs and limited features. The C code provides a generic parser capable of handling standard INI files with sections, key-value pairs, and additional features like type checking.


## Memory pooling

this project follows the concept of memory pooling objects, essentially each
object contains a list of memory pointers to free, greatly reducing the
amount of memory that needs a free function call.

## Sample File

```ini
;; comments are allowed :)
;; comments are also allowed on the same line as a value, also called
;; inline comments
[section]
a = 1 ; an inline comment

;; references are allowed
;; using \(<name>), you can reference another variable as long as it's in the same section
b = \(a)

;; lists are also allowed, each value is converted at runtime
;; as well as nested lists
c = (1, 2, 3)
c_nested = (1, (2, 3))

;; here's another section
[section-2]
b = 2

;; and keep in mind, by default, all key values are saved into a section named "_root"
```

```
section
  a = 1
  b = 1
section-2
  b = 2
```
