class Program {
    m_int : Integer;

    method arifmet() {
        a : Integer(5);
        b : Integer(7);
        c : Integer(0);
        //a.Set(5);
        //b.Set(7);

        // equal:
        // c.Set(a.Add(b));
        // c.Print();
        a.Add(b).Print();
        a.Sub(b).Print();
        a.Minus().Print();
        a.Mult(b).Print();
        a.Div(2).Print();
        a.SRem(3).Print();
    }

    method cmptest(lhs : Integer, rhs : Integer) {
        lhs.Equal(rhs).Print();
        lhs.Equal(rhs.Add(1)).Print();
        lhs.NotEqual(rhs).Print();
    }

    method Get() : Integer {
        a : Integer(10);
        //a.Set(10);
        return a;
    }

    method Program() {
        this.arifmet();
        //this.cmptest(10, 10);
    }
}
