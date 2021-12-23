package lu

import (
	"fmt"
	"time"
)

type Matrix [][]float64

func Zero(n int) Matrix {
	r := make([][]float64, n)
	a := make([]float64, n*n)
	for i := range r {
		r[i] = a[n*i : n*(i+1)]
	}
	return r
}

func Eye(n int) Matrix {
	r := Zero(n)
	for i := range r {
		r[i][i] = 1
	}
	return r
}

func (m Matrix) Print(label string) {
	if label > "" {
		fmt.Printf("%s:\n", label)
	}
	for _, r := range m {
		for _, e := range r {
			fmt.Printf(" %9.5f", e)
		}
		fmt.Println()
	}
}

func (a Matrix) Pivotize() Matrix {
	p := Eye(len(a))
	for j, r := range a {
		max := r[j]
		row := j
		for i := j; i < len(a); i++ {
			if a[i][j] > max {
				max = a[i][j]
				row = i
			}
		}
		if j != row {
			// swap rows
			p[j], p[row] = p[row], p[j]
		}
	}
	return p
}

func (m1 Matrix) Mul(m2 Matrix) Matrix {
	r := Zero(len(m1))
	for i, r1 := range m1 {
		for j := range m2 {
			for k := range m1 {
				r[i][j] += r1[k] * m2[k][j]
			}
		}
	}
	return r
}

func (a Matrix) LU() (l, u, p Matrix, elapsed time.Duration) {
	l = Zero(len(a))
	u = Zero(len(a))
	p = a.Pivotize()
	a = p.Mul(a)
	start := time.Now()
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
	elapsed = time.Since(start)
	return
}
