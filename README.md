Just a small project for learning C++.

## Synopsis
This symmetric encryption method converts each character in the original message into its ASCII value and then passes it through the parabolic function specified in the key. The returned values are then concatenated into a string. The encrypted message can then be decrypted by using the same key.

## The Key
The key is used for encryption and decryption; if for some reason you were to use this practically, you would transfer the key through an asymmetric encryption method. I tried to make an asymmetric approach that doesn't involve prime numbers, but I made several critical errors and ended up switching to a symmetric algorithm.

The formula that operates on the characters' ASCII values is stored by the key. Since the standard quadratic form just has three coefficients (yes, I'm counting the constant as a coefficient for brevity's sake), the key can easily store these coefficients. The coefficients are separated by blocks of `<<>>`.

Example key:

    -+n-'@;z\beJ>P%?V8+!B$LE[Ibu,1p9IkS<<>>%}Irs}'N^%kg}F9f<<>>WqFyh^l$ZY%x 
Since each coefficient is stored as a set of characters, the algorithm takes the sum of the characters' ASCII values and builds the intended parabolic equation. However, the `A` coefficient is computed differently to prevent potential overflow errors at low key lengths—instead of only taking the sum, the algorithm divides 1 by the sum of the ASCII values.

## Encrypting the message
The constructor generates a random key of a length defined by the user. Using the formula defined by the key, the original message's ASCII values are converted to new values that can only be decrypted using the original key.

Encryption formula (`x` is the ASCII value):

    a*(x*x) + b*x + c

Example message encryption:

    Original: Hello to anyone reading this!
    Encrypted: 101975|||142606|||152413|||152413|||156616|||45933|||163622|||156616|||45933|||137001|||155215|||170627|||156616|||155215|||142606|||45933|||160820|||142606|||137001|||141204|||148210|||155215|||145408|||45933|||163622|||146809|||148210|||162221|||47334|||

## Decrypting the message
To decrypt an encrypted message, the user must use the same key that encrypted it. Decrypting the message with the key is simple as the characters are delimited by `|||`, so each encrypted ASCII value can be converted back to its original value using the key's formula.

Decryption formula (`y` is the encrypted ASCII value):

    (sqrt (4*a*(y - c) + b*b) - b) / (2*a)

All the decrypted characters are then compiled into a single string.