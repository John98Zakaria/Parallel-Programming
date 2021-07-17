#include "../Helpers/timers.cuh"
#include "../Helpers/AllignedVectorAlocator.h"
#include <functional>
#include <cassert>
#include <immintrin.h>

template<typename T>
Aligned32Vector<T>
row_add(Aligned32Vector<T> const &matrix_a, Aligned32Vector<T> const &matrix_b, size_t row_size, size_t col_size) {
    std::function<size_t(size_t, size_t)> index = [&](size_t row_num, size_t col_num) -> size_t {
        return row_num * col_size + col_num;
    };

    Aligned32Vector<T> answer_vector(matrix_a.size());

    for (int i = 0; i < row_size; ++i) {
        for (int j = 0; j < col_size; ++j) {
            answer_vector[index(i, j)] = matrix_a[index(i, j)] + matrix_b[index(i, j)];
        }
    }


    return answer_vector;

}


template<typename T>
Aligned32Vector<T>
column_add(Aligned32Vector<T> const &matrix_a, Aligned32Vector<T> const &matrix_b, size_t row_size, size_t col_size) {
    std::function<size_t(size_t, size_t)> index = [&](size_t row_num, size_t col_num) -> size_t {
        return row_num * col_size + col_num;
    };

    Aligned32Vector<T> answer_vector(matrix_a.size());

    for (int j = 0; j < col_size; ++j) {
        for (int i = 0; i < row_size; ++i) {
            answer_vector[index(i, j)] = matrix_a[index(i, j)] + matrix_b[index(i, j)];
        }
    }


    return answer_vector;

}

template<typename T>
Aligned32Vector<T>
just_add(Aligned32Vector<T> const &matrix_a, Aligned32Vector<T> const &matrix_b, size_t row_size, size_t col_size) {

    Aligned32Vector<T> answer_vector(matrix_a.size());

    for (int i = 0; i < matrix_a.size(); ++i) {
        answer_vector[i] = matrix_a[i] + matrix_b[i];
    }

    return answer_vector;

}


Aligned32Vector<float>
AVX_Add(Aligned32Vector<float> const &matrix_a, Aligned32Vector<float> const &matrix_b, size_t row_size,
        size_t col_size) {
    Aligned32Vector<float> answer_vector(matrix_a.size());
    __m256 a_line,b_line,c_ans;
    for (int i = 0; i < matrix_a.size(); i += 8) {
         a_line = _mm256_load_ps(matrix_a.data() + i);
         b_line = _mm256_load_ps(matrix_b.data() + i);
         c_ans = _mm256_add_ps(a_line, b_line);

        _mm256_store_ps(answer_vector.data() + i, c_ans);
    }

    return answer_vector;

}


#define wrap_timer(name, function){ \
    {helpers::CpuTimer timer_scope(name);\
    timer_scope.start();\
    function     ;\
    timer_scope.print();\
}}                      ;


int main(int argc, char *argv[]) {
    size_t n = 1 << 10UL;
    Aligned32Vector<float> a(n * n);
    Aligned32Vector<float> b(n * n);

    helpers::CpuTimer row_timer("RowAdd");
    row_timer.start();
    auto c1 = row_add(a, b, n, n);
    row_timer.print();
    helpers::CpuTimer col_timer("Column_Add");
    col_timer.start();
    auto c2 = column_add(a, b, n, n);
    col_timer.print();

    wrap_timer("JustAdd", auto c3 = just_add(a, b, n, n));
    wrap_timer("AVXTimer", auto c4 = AVX_Add(a, b, n, n));


}
