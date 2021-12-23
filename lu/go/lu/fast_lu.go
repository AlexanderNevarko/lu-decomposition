package lu

func (a Matrix) LUparallel() (l, u, p Matrix, n int) {
	l = Zero(len(a))
	u = Zero(len(a))
	p = a.Pivotize()
	a = p.Mul(a)
	for j := range a {
		l[j][j] = 1
		for i := 0; i <= j; i++ {
			sum := 0.
			for k := 0; k < i; k++ {
				sum += u[k][j] * l[i][k]
			}
			u[i][j] = a[i][j] - sum
		}
		for i := j; i < len(a); i++ {
			sum := 0.
			for k := 0; k < j; k++ {
				sum += u[k][j] * l[i][k]
			}
			l[i][j] = (a[i][j] - sum) / u[j][j]
		}
	}
	return
}
