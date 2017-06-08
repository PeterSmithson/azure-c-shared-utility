// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// This file is made an integral part of tlsio_openssl_compact.c with a #include. It
// is broken out for readability. 

// 
#define SSL_ERROR__plus__WANT_READ -2
#define SSL_ERROR__plus__WANT_WRITE -3
#define SSL_ERROR__plus__HARD_FAIL -4

#define SSL_ERROR_HARD_FAIL 99
#define SSL_Good_Ptr (void*)22
#define SSL_Good_Context_Ptr (SSL_CTX*)33
#define SSL_Good_Socket 44
#define SSL_CONNECT_SUCCESS 0
#define SSL_SET_FD_SUCCESS 1
#define SSL_SET_FD_FAILURE 0
#define SSL_READ_NO_DATA 0

#define SSL_Get_IPv4_OK (uint32_t)0x11223344
#define SSL_Get_IPv4_FAIL 0

#define SSL_good_port_number  447
#define SSL_port_number_too_low -1
#define SSL_port_number_too_high 0xffff + 1
#define SSL_good_host_name "fakehost.com"
#define SSL_good_old_host_name "fakehost.com"
uint8_t* SSL_send_buffer = (uint8_t*)"111111112222222233333333";
size_t SSL_send_message_size = sizeof(SSL_send_buffer) - 1;

#define DOWORK_RECV_XFER_BUFFER_SIZE 64
#define SSL_TEST_MESSAGE_SIZE 64
#define SSL_WRITE_MAX_TEST_SIZE 60
#define SSL_SHORT_MESSAGE_SIZE 30
#define SSL_FAIL_ME_MESSAGE_SIZE 1700
const char SSL_TEST_MESSAGE[] = "0000000000111111111122222222223333333333444444444455555555556789";


// The fact that SSL_get_error requires the previous error allows a mocking strategy that
// permits encoding the extended error into the main failure
static int my_SSL_get_error(SSL* ssl, int callReturn)
{
    (void)ssl;
    switch (callReturn)
    {
    case SSL_ERROR__plus__WANT_READ:
        return SSL_ERROR_WANT_READ;
    case SSL_ERROR__plus__WANT_WRITE:
        return SSL_ERROR_WANT_WRITE;
    case SSL_ERROR__plus__HARD_FAIL:
        return SSL_ERROR_HARD_FAIL;
    }
    // This foolish-looking code dances around conflicting warnings
    // in the C and C++ compilers about no return path
    if (callReturn <= 0)
    {
        ASSERT_FAIL("bad enum");
    }
    return 0;
}

int my_SSL_read(SSL* ssl, uint8_t* buffer, size_t size)
{
    size;
    ASSERT_ARE_EQUAL(int, (int)ssl, (int)SSL_Good_Ptr);
    ASSERT_ARE_EQUAL(size_t, DOWORK_RECV_XFER_BUFFER_SIZE, sizeof(SSL_TEST_MESSAGE) - 1);
    for (int i = 0; i < DOWORK_RECV_XFER_BUFFER_SIZE; i++)
    {
        buffer[i] = (uint8_t)SSL_TEST_MESSAGE[i];
    }
    return DOWORK_RECV_XFER_BUFFER_SIZE;
}

int my_SSL_write(SSL* ssl, uint8_t* buffer, size_t size)
{
    // "Send" no more than SSL_WRITE_MAX_TEST_SIZE bytes
    (void)buffer; // not used
    ASSERT_ARE_EQUAL(int, (int)ssl, (int)SSL_Good_Ptr);
    int result;
    if (size == SSL_FAIL_ME_MESSAGE_SIZE)
    {
        result = SSL_ERROR__plus__HARD_FAIL;
    }
    else
    {
        if (size > SSL_WRITE_MAX_TEST_SIZE)
        {
            result = SSL_WRITE_MAX_TEST_SIZE;
        }
        else
        {
            result = size;
        }
    }
    return result;
}