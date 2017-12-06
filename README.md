# Kompressionsprogramm
Ein einfaches Huffman-Coding benutzendes Kompressionsprogramm. Das Programm kann sowohl mit 8-Bits lange als auch mit 16-Bits lange Aufteilung funktionieren. 16-Bit Kompression braucht mehr Zeit um durchgeführt zu werden, aber ergibt ein kompakter File am Ende.

## Anforderungen
* Linux oder BSD basiertes System
* AMD64 Architektur

## Installation

```
git clone --recursive https://github.com/mondokm/kompressionsprogramm.git
cd kompressionsprogramm
sudo make install
```

## Verwendung

```
komp -k[ompression] -d[ekompression] -8[-bit] -16[-bit] file
```
Default: 8-bit Kompression

### Kompression

Das Programm wird erstens das File Byte zu Byte durchlesen und zahlen, wie viel es von die Verschiedene Variationen gibt. Mit der gesammelten Information wird es dann ein binären Baum bauen, mithilfe von denen es die Länge der Kodes berechnen wird.

### Dekompression

### Beispiele