/** @file
 *****************************************************************************
 * @author     This file is part of libff, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#ifndef ALT_BN128_G2_HPP_
#define ALT_BN128_G2_HPP_
#include <vector>

#include <libff/algebra/curves/alt_bn128/alt_bn128_init.hpp>
#include <libff/algebra/curves/curve_utils.hpp>

namespace libff {

class alt_bn128_G2;
std::ostream& operator<<(std::ostream &, const alt_bn128_G2&);
std::istream& operator>>(std::istream &, alt_bn128_G2&);

class alt_bn128_G2 {
public:
#ifdef PROFILE_OP_COUNTS
    static long long add_cnt;
    static long long dbl_cnt;
#endif
    static std::vector<size_t> &wnaf_window_table();
    static std::vector<size_t> &fixed_base_exp_window_table();
    static alt_bn128_G2& G2_zero();
    static alt_bn128_G2& G2_one();

    typedef alt_bn128_Fq base_field;
    typedef alt_bn128_Fq2 twist_field;
    typedef alt_bn128_Fr scalar_field;

    alt_bn128_Fq2 X, Y, Z;
    alt_bn128_Fq2 &coord(int i) {
      if (i == 0)
        return X;
      else if (i == 1)
        return Y;
      else if (i == 2)
        return Z;
      else
        throw std::runtime_error("invalid index");
    }

    // using Jacobian coordinates
    alt_bn128_G2();
    struct constructor_dummy_t {};
    alt_bn128_G2(constructor_dummy_t) {}
    alt_bn128_G2(const alt_bn128_Fq2& X, const alt_bn128_Fq2& Y, const alt_bn128_Fq2& Z) : X(X), Y(Y), Z(Z) {};

    static alt_bn128_Fq2 mul_by_b(const alt_bn128_Fq2 &elt);

    void print() const;
    void print_coordinates() const;

    void to_affine_coordinates();
    void to_special();
    bool is_special() const;

    bool is_zero() const;

    bool operator==(const alt_bn128_G2 &other) const;
    bool operator!=(const alt_bn128_G2 &other) const;

    alt_bn128_G2 operator+(const alt_bn128_G2 &other) const;
    alt_bn128_G2 operator-() const;
    alt_bn128_G2 operator-(const alt_bn128_G2 &other) const;

    alt_bn128_G2 add(const alt_bn128_G2 &other) const;
    alt_bn128_G2 mixed_add(const alt_bn128_G2 &other) const;
    alt_bn128_G2 dbl() const;
    alt_bn128_G2 mul_by_q() const;

    bool is_well_formed() const;

    static alt_bn128_G2 zero();
    static alt_bn128_G2 one();
    static alt_bn128_G2 random_element();

    static size_t size_in_bits() { return twist_field::size_in_bits() + 1; }
    static bigint<base_field::num_limbs> base_field_char() { return base_field::field_char(); }
    static bigint<scalar_field::num_limbs> order() { return scalar_field::field_char(); }

    friend std::ostream& operator<<(std::ostream &out, const alt_bn128_G2 &g);
    friend std::istream& operator>>(std::istream &in, alt_bn128_G2 &g);

    static void batch_to_special_all_non_zeros(std::vector<alt_bn128_G2> &vec);
};

template<mp_size_t m>
alt_bn128_G2 operator*(const bigint<m> &lhs, const alt_bn128_G2 &rhs)
{
    return scalar_mul<alt_bn128_G2, m>(rhs, lhs);
}

template<mp_size_t m, const bigint<m>& modulus_p>
alt_bn128_G2 operator*(const Fp_model<m,modulus_p> &lhs, const alt_bn128_G2 &rhs)
{
    return scalar_mul<alt_bn128_G2, m>(rhs, lhs.as_bigint());
}


} // libff
#endif // ALT_BN128_G2_HPP_
