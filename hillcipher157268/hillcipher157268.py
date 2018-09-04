def hillcipher(message, key, n, decipher = False):
    alpha = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    tonum = dict([(alpha[i], i * 1) for i in range(len(alpha))])

    if len(message) % n > 0:
        message += 'Z' * (n - (len(message) % n))

    keylist = []
    for a in key:
        keylist.append(tonum[a])

    keymatrix = []
    for i in range(n):
        keymatrix.append(keylist[i * n : i * n + n])

    from numpy import matrix
    from numpy import linalg

    keymatrix = matrix(keymatrix)

    if decipher:
        determinant = linalg.det(keymatrix).round()
        print "Determinant", determinant
        if determinant == 0:
            raise Exception("Determinant ZERO, Change key")
        elif determinant % len(alpha) == 0:
            raise Exception("Determinant divisible by alpha length, Change key")

        inverse = []
        keymatrix = matrix(keymatrix.getI() * determinant).round()

        invdeterminant = 0
        for i in range(10000):
            if ((determinant * i) % len(alpha)) == 1:
                invdeterminant = i
                break

        if invdeterminant == 0:
            raise Exception("Modulo inverse does not exist")

        print "Inverted Determinant", invdeterminant

        for row in keymatrix.getA() * invdeterminant:
            newrow = []
            for i in row:
                newrow.append( i.round() % len(alpha) )
            inverse.append(newrow)

        keymatrix = matrix(inverse)

        print "Deciphering", message
        print
    else:
        print "Enciphering ", message
        print

    print "Key matrix\n", keymatrix
    print

    from string import join
    out = ''
    for i in range(len(message) / n):
        lst = matrix( [[tonum[a]] for a in message[i * n:i * n + n]] )
        result = keymatrix * lst
        out += ''.join([alpha[int(result.A[j][0]) % len(alpha)] for j in range(len(result))])

    return out


key = raw_input("Enter a key: ")
msg = raw_input("Enter the message: ")
n = input("Enter n: ")
print
ciphertext = hillcipher(msg, key, n)
print "Ciphered Text", ciphertext
print
print "Deciphering", ciphertext, "to check correctness"
print
decipheredtext = hillcipher(ciphertext, key, n, True)
print "Deciphered Text", decipheredtext
