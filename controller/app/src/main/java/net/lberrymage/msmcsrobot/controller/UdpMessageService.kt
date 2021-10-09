package net.lberrymage.msmcsrobot.controller

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import net.lberrymage.msmcsrobot.proto.MotorSpeed
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress

const val SERVICE_PORT = 1496

val DEST_IP: InetAddress = InetAddress.getByName("192.168.4.1")

class UdpMessageService {
    private val udpSocket = DatagramSocket()

    suspend fun sendSpeed(left: Int, right: Int) {
        val packet = MotorSpeed.newBuilder().setLeft(left).setRight(right).build().toByteArray()

        withContext(Dispatchers.IO) {
            udpSocket.send(DatagramPacket(packet, packet.size, DEST_IP, SERVICE_PORT))
        }
    }

    fun close() {
        udpSocket.close()
    }
}
