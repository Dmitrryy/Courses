
class Program {
    m_int : Integer;

    method whileTest() {
        m_int.Set(1);
        // inf loop
        while(m_int) {
            loc : Integer;
            loc.Set(66);
            loc.Print();
        }
    }

    // main
    method Program() {
        this.whileTest();
    }
}
