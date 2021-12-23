package main

import (
	"bufio"
	"bytes"
	"fmt"
	"io"
	"log"
	"os"
	"strconv"
	"strings"

	"github.com/AlexanderNevarko/lu-decomposition/lu/go/lu"
)

// type matrix [][]float64

func zero(n int) lu.Matrix {
	r := make([][]float64, n)
	a := make([]float64, n*n)
	for i := range r {
		r[i] = a[n*i : n*(i+1)]
	}
	return r
}

func lineCounter(name string) (int, error) {
	f, err := os.Open(name)
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()
	buf := make([]byte, 32*1024)
	count := 0
	lineSep := []byte{'\n'}

	for {
		c, err := f.Read(buf)
		count += bytes.Count(buf[:c], lineSep)

		switch {
		case err == io.EOF:
			return count, nil

		case err != nil:
			return count, err
		}
	}
}

func readMatrix(name string) (lu.Matrix, error) {
	n, _ := lineCounter(name)
	fmt.Println(n)
	mat := zero(n)
	f, err := os.Open(name)
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()

	scanner := bufio.NewScanner(f)
	// optionally, resize scanner's capacity for lines over 64K, see next example
	i := 0
	for scanner.Scan() {
		str := scanner.Text()
		buff := strings.Split(str, " ")
		for j, digit := range buff {
			floatDigit, errFloat := strconv.ParseFloat(digit, 32)
			if errFloat != nil {
				log.Fatal(err)
			}
			mat[i][j] = floatDigit
		}
		i++
	}
	return mat, err
}

func main() {
	args := os.Args[1:]
	matFile := args[0]
	method := args[1]
	fmt.Println(method)
	fmt.Println(matFile)
	mat, err := readMatrix(matFile)
	if err != nil {
		log.Fatal(err)
	}
	if method == "sequential" {
		_, _, _, dt := mat.LU()
		logFile, err := os.OpenFile(fmt.Sprintf("logs/go_sequential_%d", len(mat)), os.O_WRONLY|os.O_CREATE, 0666)
		if err != nil {
			log.Fatal(err)
		}
		logFile.WriteString(fmt.Sprintf("%s", dt))
		logFile.Close()
		// l, u, p := mat.LU()
		// l.Print("l")
		// u.Print("u")
		// p.Print("p")
	} else if method == "concurrent" {
		n, _ := strconv.Atoi(args[2])
		_, _, _, dt := mat.LUconcurrent(n)
		logFile, err := os.OpenFile(fmt.Sprintf("logs/go_concurrent_%d_%d", n, len(mat)), os.O_WRONLY|os.O_CREATE, 0666)
		if err != nil {
			log.Fatal(err)
		}
		logFile.WriteString(fmt.Sprintf("%s", dt))
		logFile.Close()
		// l, u, p := mat.LUconcurrent(n)
		// l.Print("l")
		// u.Print("u")
		// p.Print("p")
	} else {
		fmt.Printf("Unsupported method %s\n", method)
	}
}

// func showLU(a matrix) {
//     a.print("\na")
//     l, u, p := a.lu()
//     l.print("l")
//     u.print("u")
//     p.print("p")
// }
