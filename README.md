# sb2tools
These tools convert .sb2 files from SpongeBob SquarePants: Employee of the Month to and from .txt files.

.sb3 files from Battle for Bikini Bottom and .sbt files from The Movie game are also supported.

## sb2make
Convert .txt file to .sb2 file.

Usage:

    sb2make <input text file>
    sb2make <input text file> <output sb2 file>

## sb2text
Convert .sb2 file to .txt file.

Usage:

    sb2text <input sb2 file>
    sb2text <input sb2 file> <output text file>

## .sb2/.sb3/.sbt format
.sb2/.sb3/.sbt files contain dialogue and script commands for the game. They are basically encrypted text files, with each line separated. The format is very simple:

    int16 lineCount
    for i = 0 to lineCount:
        int16 lineLength
        char lineString[lineLength]

Each character in lineString is encrypted by XOR'ing its ASCII value with the value 0x30. Since XOR is lossless, the same string can be decrypted by XOR'ing each character with 0x30 again.
