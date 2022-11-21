class Program {
    m_int : Integer;

    method binop(lhs : Integer, rhs : Integer) {
        lhs.Or(rhs).Print();
        lhs.Not().Print();
        lhs.Xor(rhs).Print();
        lhs.And(rhs).Print();
        lhs.Shl(2).Print();
        lhs.AShr(1).Print();
    }

    method Program() {
        this.binop(4, 6);
    }
}
