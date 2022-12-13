class FibGenerator {
    m_a : Integer;
    m_b : Integer;

    method Init(a : Integer, b : Integer) {
        m_a.Set(a);
        m_b.Set(b);
    }

    method Next() : Integer {
        next : Integer(m_a.Add(m_b));
        //next.Set(m_a.Add(m_b));
        
        tmp_a : Integer(m_a);
        //tmp_a.Set(m_a);
        m_a.Set(m_b);
        m_b.Set(next);

        return tmp_a;
    }
}

class Program {
    m_a : Integer;

    method Program() {
        target : Integer(20);
        //target.Set(20);
        fg : FibGenerator(1, 1);
        //m_fg.Init(1, 1);
        while(target) {
            fg.Next().Print();

            target.Set(target.Sub(1));
        }
    }
}
