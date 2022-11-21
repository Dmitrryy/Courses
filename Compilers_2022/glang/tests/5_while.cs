
class Program {
    m_int : Integer;

    method whileTest() {
        m_int.Set(6);
        while(m_int) {
            m_int.Set(m_int.Sub(1));
            // 5 4 3 2 1 0
            m_int.Print();
        }
    }

    // main
    method Program() {
        this.whileTest();
    }
}
