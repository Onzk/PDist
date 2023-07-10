package systeme.java.metier;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface CtlInterface extends Remote {

    public String printInformations() throws RemoteException;

}
