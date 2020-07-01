mod521 = 6864797660130609714981900799081393217269435300143305409394463459185543183397656052122559640661454554977296311391480858037121987999716643812574028291115057151
mod = 115792089210356248762697446949407573530086143415290314195533631308867097853951

def inv (val) :
	res = mod - 2
	ans = 1
	while res :
		if res & 1 :
			ans *= val
			ans %= mod
		res >>= 1
		val *= val
		val %= mod
	return ans

def add (a, b, A, B) :
	if (a[0] == "inf" and a[1] == "inf") :
		return b
	flag = False
	if (a[0] == b[0] and a[1] == b[1]) :
		m = (a[0] * a[0] % mod * 3 % mod + A) % mod * inv(a[1] * 2 % mod) % mod
	else :
		if ((a[1] - b[1]) * (a[0] - b[0]) < 0) :
			flag = True
		m = abs(a[1] - b[1]) * inv(abs(a[0] - b[0])) % mod
		if flag :
			m = -m
		m %= mod
	xr = (m * m % mod - a[0] - b[0]) % mod
	return (xr, (m * (a[0] - xr) - a[1] % mod) % mod)

def mul (n, P, A, B) :
	ans = ("inf", "inf")
	while n > 0 :
		if n & 1 :
			ans = add(ans, P, A, B)
		n >>= 1
		P = add(P, P, A, B)
	return (str(hex(ans[0])), str(hex(ans[1])))

def getKey(N, X, Y, A, B) :
	res = mul(N, (int(X, 16), int(Y, 16)), A, B)
	return (res[0], res[1])

def Hello() :
	print("Hello")
	return "Hello"
