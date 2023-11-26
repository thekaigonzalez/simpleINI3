# simpleINI3

the third iteration of my simple ini

## Memory pooling

this project follows the concept of memory pooling objects, essentially each
object contains a list of memory pointers to free, greatly reducing the
amount of memory that needs a free function call.

## Sample File

```ini
;; comments are allowed :)
;; however, they are not allowed on the same line as the value
[section]
a = 1

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
