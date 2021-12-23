package lu

import "time"

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}

func (a Matrix) LUconcurrent(n int) (l, u, p Matrix, elapsed time.Duration) {
	l = Zero(len(a))
	u = Zero(len(a))
	p = a.Pivotize()
	a = p.Mul(a)

	start := time.Now()

	size := len(a)
	chunkSize := size / n
	for i := range a {
		ch1 := make(chan int, n)
		for q := 0; q < n; q += 1 {
			go func(q int) {
				for j := chunkSize * q; j < min(chunkSize*(q+1), size); j++ {
					if j < i {
						l[i][j] = 0
						continue
					}
					l[j][i] = a[j][i]
					for k := 0; k < i; k++ {
						l[j][i] = l[j][i] - l[j][k]*u[k][i]
					}
				}
				ch1 <- 1
				return
			}(q)
		}
		for q := 0; q < n; q++ {
			<-ch1
		}
		ch2 := make(chan int, n)
		for q := 0; q < n; q += 1 {
			go func(q int) {
				for j := chunkSize * q; j < min(chunkSize*(q+1), size); j++ {
					if j < i {
						u[i][j] = 0
						continue
					}
					if i == j {
						u[i][j] = 1
						continue
					}
					u[i][j] = a[i][j] / l[i][i]
					for k := 0; k < i; k++ {
						u[i][j] = u[i][j] - ((l[i][k] * u[k][j]) / l[i][i])
					}
				}
				ch2 <- 1
				return
			}(q)
		}
		for q := 0; q < n; q++ {
			<-ch2
		}
	}
	elapsed = time.Since(start)
	return
}
