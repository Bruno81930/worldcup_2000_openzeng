public class Test{

    public static void main(String arg[]){

	double x = new Double(arg[0]).doubleValue();
	double y = new Double(arg[1]).doubleValue();

	System.out.println(Math.atan2(y,x));

    }
	
}

