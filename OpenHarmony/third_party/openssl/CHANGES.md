OpenSSL CHANGES
===============

This is a high-level summary of the most important changes.
For a full list of changes, see the [git commit log][log] and
pick the appropriate release branch.

  [log]: https://github.com/openssl/openssl/commits/

OpenSSL Releases
----------------

 - [OpenSSL 3.0](#openssl-30)
 - [OpenSSL 1.1.1](#openssl-111)
 - [OpenSSL 1.1.0](#openssl-110)
 - [OpenSSL 1.0.2](#openssl-102)
 - [OpenSSL 1.0.1](#openssl-101)
 - [OpenSSL 1.0.0](#openssl-100)
 - [OpenSSL 0.9.x](#openssl-09x)

OpenSSL 3.0
-----------

For OpenSSL 3.0 a [Migration guide][] has been added, so the CHANGES entries
listed here are only a brief description.
The migration guide contains more detailed information related to new features,
breaking changes, and mappings for the large list of deprecated functions.

[Migration guide]: https://github.com/openssl/openssl/tree/master/doc/man7/migration_guide.pod

### Changes between 3.0.8 and 3.0.9 [30 May 2023]

 * Mitigate for the time it takes for `OBJ_obj2txt` to translate gigantic
   OBJECT IDENTIFIER sub-identifiers to canonical numeric text form.

   OBJ_obj2txt() would translate any size OBJECT IDENTIFIER to canonical
   numeric text form.  For gigantic sub-identifiers, this would take a very
   long time, the time complexity being O(n^2) where n is the size of that
   sub-identifier.  ([CVE-2023-2650])

   To mitigitate this, `OBJ_obj2txt()` will only translate an OBJECT
   IDENTIFIER to canonical numeric text form if the size of that OBJECT
   IDENTIFIER is 586 bytes or less, and fail otherwise.

   The basis for this restriction is RFC 2578 (STD 58), section 3.5. OBJECT
   IDENTIFIER values, which stipulates that OBJECT IDENTIFIERS may have at
   most 128 sub-identifiers, and that the maximum value that each sub-
   identifier may have is 2^32-1 (4294967295 decimal).

   For each byte of every sub-identifier, only the 7 lower bits are part of
   the value, so the maximum amount of bytes that an OBJECT IDENTIFIER with
   these restrictions may occupy is 32 * 128 / 7, which is approximately 586
   bytes.

   Ref: https://datatracker.ietf.org/doc/html/rfc2578#section-3.5

   *Richard Levitte*

 * Fixed buffer overread in AES-XTS decryption on ARM 64 bit platforms which
   happens if the buffer size is 4 mod 5 in 16 byte AES blocks. This can
   trigger a crash of an application using AES-XTS decryption if the memory
   just after the buffer being decrypted is not mapped.
   Thanks to Anton Romanov (Amazon) for discovering the issue.
   ([CVE-2023-1255])

   *Nevine Ebeid*

 * Reworked the Fix for the Timing Oracle in RSA Decryption ([CVE-2022-4304]).
   The previous fix for this timing side channel turned out to cause
   a severe 2-3x performance regression in the typical use case
   compared to 3.0.7. The new fix uses existing constant time
   code paths, and restores the previous performance level while
   fully eliminating all existing timing side channels.
   The fix was developed by Bernd Edlinger with testing support
   by Hubert Kario.

   *Bernd Edlinger*

 * Corrected documentation of X509_VERIFY_PARAM_add0_policy() to mention
   that it does not enable policy checking. Thanks to David Benjamin for
   discovering this issue.
   ([CVE-2023-0466])

   *Tomáš Mráz*

 * Fixed an issue where invalid certificate policies in leaf certificates are
   silently ignored by OpenSSL and other certificate policy checks are skipped
   for that certificate. A malicious CA could use this to deliberately assert
   invalid certificate policies in order to circumvent policy checking on the
   certificate altogether.
   ([CVE-2023-0465])

   *Matt Caswell*

 * Limited the number of nodes created in a policy tree to mitigate
   against CVE-2023-0464.  The default limit is set to 1000 nodes, which
   should be sufficient for most installations.  If required, the limit
   can be adjusted by setting the OPENSSL_POLICY_TREE_NODES_MAX build
   time define to a desired maximum number of nodes or zero to allow
   unlimited growth.
   ([CVE-2023-0464])

   *Paul Dale*

### Changes between 3.0.7 and 3.0.8 [7 Feb 2023]

 * Fixed NULL dereference during PKCS7 data verification.

   A NULL pointer can be dereferenced when signatures are being
   verified on PKCS7 signed or signedAndEnveloped data. In case the hash
   algorithm used for the signature is known to the OpenSSL library but
   the implementation of the hash algorithm is not available the digest
   initialization will fail. There is a missing check for the return
   value from the initialization function which later leads to invalid
   usage of the digest API most likely leading to a crash.
   ([CVE-2023-0401])

   PKCS7 data is processed by the SMIME library calls and also by the
   time stamp (TS) library calls. The TLS implementation in OpenSSL does
   not call these functions however third party applications would be
   affected if they call these functions to verify signatures on untrusted
   data.

   *Tomáš Mráz*

 * Fixed X.400 address type confusion in X.509 GeneralName.

   There is a type confusion vulnerability relating to X.400 address processing
   inside an X.509 GeneralName. X.400 addresses were parsed as an ASN1_STRING
   but the public structure definition for GENERAL_NAME incorrectly specified
   the type of the x400Address field as ASN1_TYPE. This field is subsequently
   interpreted by the OpenSSL function GENERAL_NAME_cmp as an ASN1_TYPE rather
   than an ASN1_STRING.

   When CRL checking is enabled (i.e. the application sets the
   X509_V_FLAG_CRL_CHECK flag), this vulnerability may allow an attacker to
   pass arbitrary pointers to a memcmp call, enabling them to read memory
   contents or enact a denial of service.
   ([CVE-2023-0286])

   *Hugo Landau*

 * Fixed NULL dereference validating DSA public key.

   An invalid pointer dereference on read can be triggered when an
   application tries to check a malformed DSA public key by the
   EVP_PKEY_public_check() function. This will most likely lead
   to an application crash. This function can be called on public
   keys supplied from untrusted sources which could allow an attacker
   to cause a denial of service attack.

   The TLS implementation in OpenSSL does not call this function
   but applications might call the function if there are additional
   security requirements imposed by standards such as FIPS 140-3.
   ([CVE-2023-0217])

   *Shane Lontis, Tomáš Mráz*

 * Fixed Invalid pointer dereference in d2i_PKCS7 functions.

   An invalid pointer dereference on read can be triggered when an
   application tries to load malformed PKCS7 data with the
   d2i_PKCS7(), d2i_PKCS7_bio() or d2i_PKCS7_fp() functions.

   The result of the dereference is an application crash which could
   lead to a denial of service attack. The TLS implementation in OpenSSL
   does not call this function however third party applications might
   call these functions on untrusted data.
   ([CVE-2023-0216])

   *Tomáš Mráz*

 * Fixed Use-after-free following BIO_new_NDEF.

   The public API function BIO_new_NDEF is a helper function used for
   streaming ASN.1 data via a BIO. It is primarily used internally to OpenSSL
   to support the SMIME, CMS and PKCS7 streaming capabilities, but may also
   be called directly by end user applications.

   The function receives a BIO from the caller, prepends a new BIO_f_asn1
   filter BIO onto the front of it to form a BIO chain, and then returns
   the new head of the BIO chain to the caller. Under certain conditions,
   for example if a CMS recipient public key is invalid, the new filter BIO
   is freed and the function returns a NULL result indicating a failure.
   However, in this case, the BIO chain is not properly cleaned up and the
   BIO passed by the caller still retains internal pointers to the previously
   freed filter BIO. If the caller then goes on to call BIO_pop() on the BIO
   then a use-after-free will occur. This will most likely result in a crash.
   ([CVE-2023-0215])

   *Viktor Dukhovni, Matt Caswell*

 * Fixed Double free after calling PEM_read_bio_ex.

   The function PEM_read_bio_ex() reads a PEM file from a BIO and parses and
   decodes the "name" (e.g. "CERTIFICATE"), any header data and the payload
   data. If the function succeeds then the "name_out", "header" and "data"
   arguments are populated with pointers to buffers containing the relevant
   decoded data. The caller is responsible for freeing those buffers. It is
   possible to construct a PEM file that results in 0 bytes of payload data.
   In this case PEM_read_bio_ex() will return a failure code but will populate
   the header argument with a pointer to a buffer that has already been freed.
   If the caller also frees this buffer then a double free will occur. This
   will most likely lead to a crash.

   The functions PEM_read_bio() and PEM_read() are simple wrappers around
   PEM_read_bio_ex() and therefore these functions are also directly affected.

   These functions are also called indirectly by a number of other OpenSSL
   functions including PEM_X509_INFO_read_bio_ex() and
   SSL_CTX_use_serverinfo_file() which are also vulnerable. Some OpenSSL
   internal uses of these functions are not vulnerable because the caller does
   not free the header argument if PEM_read_bio_ex() returns a failure code.
   ([CVE-2022-4450])

   *Kurt Roeckx, Matt Caswell*

 * Fixed Timing Oracle in RSA Decryption.

   A timing based side channel exists in the OpenSSL RSA Decryption
   implementation which could be sufficient to recover a plaintext across
   a network in a Bleichenbacher style attack. To achieve a successful
   decryption an attacker would have to be able to send a very large number
   of trial messages for decryption. The vulnerability affects all RSA padding
   modes: PKCS#1 v1.5, RSA-OEAP and RSASVE.
   ([CVE-2022-4304])

   *Dmitry Belyavsky, Hubert Kario*

 * Fixed X.509 Name Constraints Read Buffer Overflow.

   A read buffer overrun can be triggered in X.509 certificate verification,
   specifically in name constraint checking. The read buffer overrun might
   result in a crash which could lead to a denial of service attack.
   In a TLS client, this can be triggered by connecting to a malicious
   server. In a TLS server, this can be triggered if the server requests
   client authentication and a malicious client connects.
   ([CVE-2022-4203])

   *Viktor Dukhovni*

 * Fixed X.509 Policy Constraints Double Locking security issue.

   If an X.509 certificate contains a malformed policy constraint and
   policy processing is enabled, then a write lock will be taken twice
   recursively.  On some operating systems (most widely: Windows) this
   results in a denial of service when the affected process hangs.  Policy
   processing being enabled on a publicly facing server is not considered
   to be a common setup.
   ([CVE-2022-3996])

   *Paul Dale*

 * Our provider implementations of `OSSL_FUNC_KEYMGMT_EXPORT` and
   `OSSL_FUNC_KEYMGMT_GET_PARAMS` for EC and SM2 keys now honor
   `OSSL_PKEY_PARAM_EC_POINT_CONVERSION_FORMAT` as set (and
   default to `POINT_CONVERSION_UNCOMPRESSED`) when exporting
   `OSSL_PKEY_PARAM_PUB_KEY`, instead of unconditionally using
   `POINT_CONVERSION_COMPRESSED` as in previous 3.x releases.
   For symmetry, our implementation of `EVP_PKEY_ASN1_METHOD->export_to`
   for legacy EC and SM2 keys is also changed similarly to honor the
   equivalent conversion format flag as specified in the underlying
   `EC_KEY` object being exported to a provider, when this function is
   called through `EVP_PKEY_export()`.

   *Nicola Tuveri*

### Changes between 3.0.6 and 3.0.7 [1 Nov 2022]

 * Fixed two buffer overflows in punycode decoding functions.

   A buffer overrun can be triggered in X.509 certificate verification,
   specifically in name constraint checking. Note that this occurs after
   certificate chain signature verification and requires either a CA to
   have signed the malicious certificate or for the application to continue
   certificate verification despite failure to construct a path to a trusted
   issuer.

   In a TLS client, this can be triggered by connecting to a malicious
   server.  In a TLS server, this can be triggered if the server requests
   client authentication and a malicious client connects.

   An attacker can craft a malicious email address to overflow
   an arbitrary number of bytes containing the `.`  character (decimal 46)
   on the stack.  This buffer overflow could result in a crash (causing a
   denial of service).
   ([CVE-2022-3786])

   An attacker can craft a malicious email address to overflow four
   attacker-controlled bytes on the stack.  This buffer overflow could
   result in a crash (causing a denial of service) or potentially remote code
   execution depending on stack layout for any given platform/compiler.
   ([CVE-2022-3602])

   *Paul Dale*

 * Removed all references to invalid OSSL_PKEY_PARAM_RSA names for CRT
   parameters in OpenSSL code.
   Applications should not use the names OSSL_PKEY_PARAM_RSA_FACTOR,
   OSSL_PKEY_PARAM_RSA_EXPONENT and OSSL_PKEY_PARAM_RSA_COEFFICIENT.
   Use the numbered names such as OSSL_PKEY_PARAM_RSA_FACTOR1 instead.
   Using these invalid names may cause algorithms to use slower methods
   that ignore the CRT parameters.

   *Shane Lontis*

 * Fixed a regression introduced in 3.0.6 version raising errors on some stack
   operations.

   *Tomáš Mráz*

 * Fixed a regression introduced in 3.0.6 version not refreshing the certificate
   data to be signed before signing the certificate.

   *Gibeom Gwon*

 * Added RIPEMD160 to the default provider.

   *Paul Dale*

 * Ensured that the key share group sent or accepted for the key exchange
   is allowed for the protocol version.

   *Matt Caswell*

### Changes between 3.0.5 and 3.0.6 [11 Oct 2022]

 * OpenSSL supports creating a custom cipher via the legacy
   EVP_CIPHER_meth_new() function and associated function calls. This function
   was deprecated in OpenSSL 3.0 and application authors are instead encouraged
   to use the new provider mechanism in order to implement custom ciphers.

   OpenSSL versions 3.0.0 to 3.0.5 incorrectly handle legacy custom ciphers
   passed to the EVP_EncryptInit_ex2(), EVP_DecryptInit_ex2() and
   EVP_CipherInit_ex2() functions (as well as other similarly named encryption
   and decryption initialisation functions). Instead of using the custom cipher
   directly it incorrectly tries to fetch an equivalent cipher from the
   available providers. An equivalent cipher is found based on the NID passed to
   EVP_CIPHER_meth_new(). This NID is supposed to represent the unique NID for a
   given cipher. However it is possible for an application to incorrectly pass
   NID_undef as this value in the call to EVP_CIPHER_meth_new(). When NID_undef
   is used in this way the OpenSSL encryption/decryption initialisation function
   will match the NULL cipher as being equivalent and will fetch this from the
   available providers. This will succeed if the default provider has been
   loaded (or if a third party provider has been loaded that offers this
   cipher). Using the NULL cipher means that the plaintext is emitted as the
   ciphertext.

   Applications are only affected by this issue if they call
   EVP_CIPHER_meth_new() using NID_undef and subsequently use it in a call to an
   encryption/decryption initialisation function. Applications that only use
   SSL/TLS are not impacted by this issue.
   ([CVE-2022-3358])

   *Matt Caswell*

 * Fix LLVM vs Apple LLVM version numbering confusion that caused build failures
   on MacOS 10.11

   *Richard Levitte*

 * Fixed the linux-mips64 Configure target which was missing the
   SIXTY_FOUR_BIT bn_ops flag. This was causing heap corruption on that
   platform.

   *Adam Joseph*

 * Fix handling of a ticket key callback that returns 0 in TLSv1.3 to not send a
   ticket

   *Matt Caswell*

 * Correctly handle a retransmitted ClientHello in DTLS

   *Matt Caswell*

 * Fixed detection of ktls support in cross-compile environment on Linux

   *Tomas Mraz*

 * Fixed some regressions and test failures when running the 3.0.0 FIPS provider
   against 3.0.x

   *Paul Dale*

 * Fixed SSL_pending() and SSL_has_pending() with DTLS which were failing to
   report correct results in some cases

   *Matt Caswell*

 * Fix UWP builds by defining VirtualLock

   *Charles Milette*

 * For known safe primes use the minimum key length according to RFC 7919.
   Longer private key sizes unnecessarily raise the cycles needed to compute the
   shared secret without any increase of the real security. This fixes a
   regression from 1.1.1 where these shorter keys were generated for the known
   safe primes.

   *Tomas Mraz*

 * Added the loongarch64 target

   *Shi Pujin*

 * Fixed EC ASM flag passing. Flags for ASM implementations of EC curves were
   only passed to the FIPS provider and not to the default or legacy provider.

   *Juergen Christ*

 * Fixed reported performance degradation on aarch64. Restored the
   implementation prior to commit 2621751 ("aes/asm/aesv8-armx.pl: avoid
   32-bit lane assignment in CTR mode") for 64bit targets only, since it is
   reportedly 2-17% slower and the silicon errata only affects 32bit targets.
   The new algorithm is still used for 32 bit targets.

   *Bernd Edlinger*

 * Added a missing header for memcmp that caused compilation failure on some
   platforms

   *Gregor Jasny*

### Changes between 3.0.4 and 3.0.5 [5 Jul 2022]

 * The OpenSSL 3.0.4 release introduced a serious bug in the RSA
   implementation for X86_64 CPUs supporting the AVX512IFMA instructions.
   This issue makes the RSA implementation with 2048 bit private keys
   incorrect on such machines and memory corruption will happen during
   the computation. As a consequence of the memory corruption an attacker
   may be able to trigger a remote code execution on the machine performing
   the computation.

   SSL/TLS servers or other servers using 2048 bit RSA private keys running
   on machines supporting AVX512IFMA instructions of the X86_64 architecture
   are affected by this issue.
   ([CVE-2022-2274])

   *Xi Ruoyao*

 * AES OCB mode for 32-bit x86 platforms using the AES-NI assembly optimised
   implementation would not encrypt the entirety of the data under some
   circumstances.  This could reveal sixteen bytes of data that was
   preexisting in the memory that wasn't written.  In the special case of
   "in place" encryption, sixteen bytes of the plaintext would be revealed.

   Since OpenSSL does not support OCB based cipher suites for TLS and DTLS,
   they are both unaffected.
   ([CVE-2022-2097])

   *Alex Chernyakhovsky, David Benjamin, Alejandro Sedeño*

### Changes between 3.0.3 and 3.0.4 [21 Jun 2022]

 * In addition to the c_rehash shell command injection identified in
   CVE-2022-1292, further bugs where the c_rehash script does not
   properly sanitise shell metacharacters to prevent command injection have been
   fixed.

   When the CVE-2022-1292 was fixed it was not discovered that there
   are other places in the script where the file names of certificates
   being hashed were possibly passed to a command executed through the shell.

   This script is distributed by some operating systems in a manner where
   it is automatically executed.  On such operating systems, an attacker
   could execute arbitrary commands with the privileges of the script.

   Use of the c_rehash script is considered obsolete and should be replaced
   by the OpenSSL rehash command line tool.
   (CVE-2022-2068)

   *Daniel Fiala, Tomáš Mráz*

 * Case insensitive string comparison no longer uses locales.  It has instead
   been directly implemented.

   *Paul Dale*

### Changes between 3.0.2 and 3.0.3 [3 May 2022]

 * Case insensitive string comparison is reimplemented via new locale-agnostic
   comparison functions OPENSSL_str[n]casecmp always using the POSIX locale for
   comparison. The previous implementation had problems when the Turkish locale
   was used.

   *Dmitry Belyavskiy*

 * Fixed a bug in the c_rehash script which was not properly sanitising shell
   metacharacters to prevent command injection.  This script is distributed by
   some operating systems in a manner where it is automatically executed.  On
   such operating systems, an attacker could execute arbitrary commands with the
   privileges of the script.

   Use of the c_rehash script is considered obsolete and should be replaced
   by the OpenSSL rehash command line tool.
   (CVE-2022-1292)

   *Tomáš Mráz*

 * Fixed a bug in the function `OCSP_basic_verify` that verifies the signer
   certificate on an OCSP response. The bug caused the function in the case
   where the (non-default) flag OCSP_NOCHECKS is used to return a postivie
   response (meaning a successful verification) even in the case where the
   response signing certificate fails to verify.

   It is anticipated that most users of `OCSP_basic_verify` will not use the
   OCSP_NOCHECKS flag. In this case the `OCSP_basic_verify` function will return
   a negative value (indicating a fatal error) in the case of a certificate
   verification failure. The normal expected return value in this case would be
   0.

   This issue also impacts the command line OpenSSL "ocsp" application. When
   verifying an ocsp response with the "-no_cert_checks" option the command line
   application will report that the verification is successful even though it
   has in fact failed. In this case the incorrect successful response will also
   be accompanied by error messages showing the failure and contradicting the
   apparently successful result.
   ([CVE-2022-1343])

   *Matt Caswell*

 * Fixed a bug where the RC4-MD5 ciphersuite incorrectly used the
   AAD data as the MAC key. This made the MAC key trivially predictable.

   An attacker could exploit this issue by performing a man-in-the-middle attack
   to modify data being sent from one endpoint to an OpenSSL 3.0 recipient such
   that the modified data would still pass the MAC integrity check.

   Note that data sent from an OpenSSL 3.0 endpoint to a non-OpenSSL 3.0
   endpoint will always be rejected by the recipient and the connection will
   fail at that point. Many application protocols require data to be sent from
   the client to the server first. Therefore, in such a case, only an OpenSSL
   3.0 server would be impacted when talking to a non-OpenSSL 3.0 client.

   If both endpoints are OpenSSL 3.0 then the attacker could modify data being
   sent in both directions. In this case both clients and servers could be
   affected, regardless of the application protocol.

   Note that in the absence of an attacker this bug means that an OpenSSL 3.0
   endpoint communicating with a non-OpenSSL 3.0 endpoint will fail to complete
   the handshake when using this ciphersuite.

   The confidentiality of data is not impacted by this issue, i.e. an attacker
   cannot decrypt data that has been encrypted using this ciphersuite - they can
   only modify it.

   In order for this attack to work both endpoints must legitimately negotiate
   the RC4-MD5 ciphersuite. This ciphersuite is not compiled by default in
   OpenSSL 3.0, and is not available within the default provider or the default
   ciphersuite list. This ciphersuite will never be used if TLSv1.3 has been
   negotiated. In order for an OpenSSL 3.0 endpoint to use this ciphersuite the
   following must have occurred:

   1) OpenSSL must have been compiled with the (non-default) compile time option
      enable-weak-ssl-ciphers

   2) OpenSSL must have had the legacy provider explicitly loaded (either
      through application code or via configuration)

   3) The ciphersuite must have been explicitly added to the ciphersuite list

   4) The libssl security level must have been set to 0 (default is 1)

   5) A version of SSL/TLS below TLSv1.3 must have been negotiated

   6) Both endpoints must negotiate the RC4-MD5 ciphersuite in preference to any
      others that both endpoints have in common
   (CVE-2022-1434)

   *Matt Caswell*

 * Fix a bug in the OPENSSL_LH_flush() function that breaks reuse of the memory
   occuppied by the removed hash table entries.

   This function is used when decoding certificates or keys. If a long lived
   process periodically decodes certificates or keys its memory usage will
   expand without bounds and the process might be terminated by the operating
   system causing a denial of service. Also traversing the empty hash table
   entries will take increasingly more time.

   Typically such long lived processes might be TLS clients or TLS servers
   configured to accept client certificate authentication.
   (CVE-2022-1473)

   *Hugo Landau, Aliaksei Levin*

 * The functions `OPENSSL_LH_stats` and `OPENSSL_LH_stats_bio` now only report
   the `num_items`, `num_nodes` and `num_alloc_nodes` statistics. All other
   statistics are no longer supported. For compatibility, these statistics are
   still listed in the output but are now always reported as zero.

   *Hugo Landau*

### Changes between 3.0.1 and 3.0.2 [15 Mar 2022]

 * Fixed a bug in the BN_mod_sqrt() function that can cause it to loop forever
   for non-prime moduli.

   Internally this function is used when parsing certificates that contain
   elliptic curve public keys in compressed form or explicit elliptic curve
   parameters with a base point encoded in compressed form.

   It is possible to trigger the infinite loop by crafting a certificate that
   has invalid explicit curve parameters.

   Since certificate parsing happens prior to verification of the certificate
   signature, any process that parses an externally supplied certificate may thus
   be subject to a denial of service attack. The infinite loop can also be
   reached when parsing crafted private keys as they can contain explicit
   elliptic curve parameters.

   Thus vulnerable situations include:

    - TLS clients consuming server certificates
    - TLS servers consuming client certificates
    - Hosting providers taking certificates or private keys from customers
    - Certificate authorities parsing certification requests from subscribers
    - Anything else which parses ASN.1 elliptic curve parameters

   Also any other applications that use the BN_mod_sqrt() where the attacker
   can control the parameter values are vulnerable to this DoS issue.
   ([CVE-2022-0778])

   *Tomáš Mráz*

 * Add ciphersuites based on DHE_PSK (RFC 4279) and ECDHE_PSK (RFC 5489)
   to the list of ciphersuites providing Perfect Forward Secrecy as
   required by SECLEVEL >= 3.

   *Dmitry Belyavskiy, Nicola Tuveri*

 * Made the AES constant time code for no-asm configurations
   optional due to the resulting 95% performance degradation.
   The AES constant time code can be enabled, for no assembly
   builds, with: ./config no-asm -DOPENSSL_AES_CONST_TIME

   *Paul Dale*

 * Fixed PEM_write_bio_PKCS8PrivateKey() to make it possible to use empty
   passphrase strings.

   *Darshan Sen*

 * The negative return value handling of the certificate verification callback
   was reverted. The replacement is to set the verification retry state with
   the SSL_set_retry_verify() function.

   *Tomáš Mráz*

### Changes between 3.0.0 and 3.0.1 [14 Dec 2021]

 * Fixed invalid handling of X509_verify_cert() internal errors in libssl
   Internally libssl in OpenSSL calls X509_verify_cert() on the client side to
   verify a certificate supplied by a server. That function may return a
   negative return value to indicate an internal error (for example out of
   memory). Such a negative return value is mishandled by OpenSSL and will cause
   an IO function (such as SSL_connect() or SSL_do_handshake()) to not indicate
   success and a subsequent call to SSL_get_error() to return the value
   SSL_ERROR_WANT_RETRY_VERIFY. This return value is only supposed to be
   returned by OpenSSL if the application has previously called
   SSL_CTX_set_cert_verify_callback(). Since most applications do not do this
   the SSL_ERROR_WANT_RETRY_VERIFY return value from SSL_get_error() will be
   totally unexpected and applications may not behave correctly as a result. The
   exact behaviour will depend on the application but it could result in
   crashes, infinite loops or other similar incorrect responses.

   This issue is made more serious in combination with a separate bug in OpenSSL
   3.0 that will cause X509_verify_cert() to indicate an internal error when
   processing a certificate chain. This will occur where a certificate does not
   include the Subject Alternative Name extension but where a Certificate
   Authority has enforced name constraints. This issue can occur even with valid
   chains.
   ([CVE-2021-4044])

   *Matt Caswell*

 * Corrected a few file name and file reference bugs in the build,
   installation and setup scripts, which lead to installation verification
   failures.  Slightly enhanced the installation verification script.

   *Richard Levitte*

 * Fixed EVP_PKEY_eq() to make it possible to use it with strictly private
   keys.

   *Richard Levitte*

 * Fixed PVK encoder to properly query for the passphrase.

   *Tomáš Mráz*

 * Multiple fixes in the OSSL_HTTP API functions.

   *David von Oheimb*

 * Allow sign extension in OSSL_PARAM_allocate_from_text() for the
   OSSL_PARAM_INTEGER data type and return error on negative numbers
   used with the OSSL_PARAM_UNSIGNED_INTEGER data type. Make
   OSSL_PARAM_BLD_push_BN{,_pad}() return an error on negative numbers.

   *Richard Levitte*

 * Allow copying uninitialized digest contexts with EVP_MD_CTX_copy_ex.

   *Tomáš Mráz*

 * Fixed detection of ARMv7 and ARM64 CPU features on FreeBSD.

   *Allan Jude*

 * Multiple threading fixes.

   *Matt Caswell*

 * Added NULL digest implementation to keep compatibility with 1.1.1 version.

   *Tomáš Mráz*

 * Allow fetching an operation from the provider that owns an unexportable key
   as a fallback if that is still allowed by the property query.

   *Richard Levitte*

### Changes between 1.1.1 and 3.0.0 [7 sep 2021]

 * TLS_MAX_VERSION, DTLS_MAX_VERSION and DTLS_MIN_VERSION constants are now
   deprecated.

   *Matt Caswell*

 * The `OPENSSL_s390xcap` environment variable can be used to set bits in the
   S390X capability vector to zero. This simplifies testing of different code
   paths on S390X architecture.

   *Patrick Steuer*

 * Encrypting more than 2^64 TLS records with AES-GCM is disallowed
   as per FIPS 140-2 IG A.5 "Key/IV Pair Uniqueness Requirements from
   SP 800-38D". The communication will fail at this point.

   *Paul Dale*

 * The EC_GROUP_clear_free() function is deprecated as there is nothing
   confidential in EC_GROUP data.

   *Nicola Tuveri*

 * The byte order mark (BOM) character is ignored if encountered at the
   beginning of a PEM-formatted file.

   *Dmitry Belyavskiy*

 * Added CMS support for the Russian GOST algorithms.

   *Dmitry Belyavskiy*

 * Due to move of the implementation of cryptographic operations
   to the providers, validation of various operation parameters can
   be postponed until the actual operation is executed where previously
   it happened immediately when an operation parameter was set.

   For example when setting an unsupported curve with
   EVP_PKEY_CTX_set_ec_paramgen_curve_nid() this function call will not
   fail but later keygen operations with the EVP_PKEY_CTX will fail.

   *OpenSSL team members and many third party contributors*

 * The EVP_get_cipherbyname() function will return NULL for algorithms such as
   "AES-128-SIV", "AES-128-CBC-CTS" and "CAMELLIA-128-CBC-CTS" which were
   previously only accessible via low level interfaces. Use EVP_CIPHER_fetch()
   instead to retrieve these algorithms from a provider.

   *Shane Lontis*

 * On build targets where the multilib postfix is set in the build
   configuration the libdir directory was changing based on whether
   the lib directory with the multilib postfix exists on the system
   or not. This unpredictable behavior was removed and eventual
   multilib postfix is now always added to the default libdir. Use
   `--libdir=lib` to override the libdir if adding the postfix is
   undesirable.

   *Jan Lána*

 * The triple DES key wrap functionality now conforms to RFC 3217 but is
   no longer interoperable with OpenSSL 1.1.1.

   *Paul Dale*

 * The ERR_GET_FUNC() function was removed.  With the loss of meaningful
   function codes, this function can only cause problems for calling
   applications.

   *Paul Dale*

 * Add a configurable flag to output date formats as ISO 8601. Does not
   change the default date format.

   *William Edmisten*

 * Version of MSVC earlier than 1300 could get link warnings, which could
   be suppressed if the undocumented -DI_CAN_LIVE_WITH_LNK4049 was set.
   Support for this flag has been removed.

   *Rich Salz*

 * Rework and make DEBUG macros consistent. Remove unused -DCONF_DEBUG,
   -DBN_CTX_DEBUG, and REF_PRINT. Add a new tracing category and use it for
   printing reference counts. Rename -DDEBUG_UNUSED to -DUNUSED_RESULT_DEBUG
   Fix BN_DEBUG_RAND so it compiles and, when set, force DEBUG_RAND to be set
   also. Rename engine_debug_ref to be ENGINE_REF_PRINT also for consistency.

   *Rich Salz*

 * The signatures of the functions to get and set options on SSL and
   SSL_CTX objects changed from "unsigned long" to "uint64_t" type.
   Some source code changes may be required.

   *Rich Salz*

 * The public definitions of conf_method_st and conf_st have been
   deprecated. They will be made opaque in a future release.

   *Rich Salz and Tomáš Mráz*

 * Client-initiated renegotiation is disabled by default. To allow it, use
   the -client_renegotiation option, the SSL_OP_ALLOW_CLIENT_RENEGOTIATION
   flag, or the "ClientRenegotiation" config parameter as appropriate.

   *Rich Salz*

 * Add "abspath" and "includedir" pragma's to config files, to prevent,
   or modify relative pathname inclusion.

   *Rich Salz*

 * OpenSSL includes a cryptographic module that is intended to be FIPS 140-2
   validated. Please consult the README-FIPS and
   README-PROVIDERS files, as well as the migration guide.

   *OpenSSL team members and many third party contributors*

 * For the key types DH and DHX the allowed settable parameters are now different.

   *Shane Lontis*

 * The openssl commands that read keys, certificates, and CRLs now
   automatically detect the PEM or DER format of the input files.

   *David von Oheimb, Richard Levitte, and Tomáš Mráz*

 * Added enhanced PKCS#12 APIs which accept a library context.

   *Jon Spillett*

 * The default manual page suffix ($MANSUFFIX) has been changed to "ossl"

   *Matt Caswell*

 * Added support for Kernel TLS (KTLS).

   *Boris Pismenny, John Baldwin and Andrew Gallatin*

 * Support for RFC 5746 secure renegotiation is now required by default for
   SSL or TLS connections to succeed.

   *Benjamin Kaduk*

 * The signature of the `copy` functional parameter of the
   EVP_PKEY_meth_set_copy() function has changed so its `src` argument is
   now `const EVP_PKEY_CTX *` instead of `EVP_PKEY_CTX *`. Similarly
   the signature of the `pub_decode` functional parameter of the
   EVP_PKEY_asn1_set_public() function has changed so its `pub` argument is
   now `const X509_PUBKEY *` instead of `X509_PUBKEY *`.

   *David von Oheimb*

 * The error return values from some control calls (ctrl) have changed.

   *Paul Dale*

 * A public key check is now performed during EVP_PKEY_derive_set_peer().

   *Shane Lontis*

 * Many functions in the EVP_ namespace that are getters of values from
   implementations or contexts were renamed to include get or get0 in their
   names. Old names are provided as macro aliases for compatibility and
   are not deprecated.

   *Tomáš Mráz*

 * The EVP_PKEY_CTRL_PKCS7_ENCRYPT, EVP_PKEY_CTRL_PKCS7_DECRYPT,
   EVP_PKEY_CTRL_PKCS7_SIGN, EVP_PKEY_CTRL_CMS_ENCRYPT,
   EVP_PKEY_CTRL_CMS_DECRYPT, and EVP_PKEY_CTRL_CMS_SIGN control operations
   are deprecated.

   *Tomáš Mráz*

 * The EVP_PKEY_public_check() and EVP_PKEY_param_check() functions now work for
   more key types.

 * The output from the command line applications may have minor
   changes.

   *Paul Dale*

 * The output from numerous "printing" may have minor changes.

   *David von Oheimb*

 * Windows thread synchronization uses read/write primitives (SRWLock) when
   supported by the OS, otherwise CriticalSection continues to be used.

   *Vincent Drake*

 * Add filter BIO BIO_f_readbuffer() that allows BIO_tell() and BIO_seek() to
   work on read only BIO source/sinks that do not support these functions.
   This allows piping or redirection of a file BIO using stdin to be buffered
   into memory. This is used internally in OSSL_DECODER_from_bio().

   *Shane Lontis*

 * OSSL_STORE_INFO_get_type() may now return an additional value. In 1.1.1
   this function would return one of the values OSSL_STORE_INFO_NAME,
   OSSL_STORE_INFO_PKEY, OSSL_STORE_INFO_PARAMS, OSSL_STORE_INFO_CERT or
   OSSL_STORE_INFO_CRL. Decoded public keys would previously have been reported
   as type OSSL_STORE_INFO_PKEY in 1.1.1. In 3.0 decoded public keys are now
   reported as having the new type OSSL_STORE_INFO_PUBKEY. Applications
   using this function should be amended to handle the changed return value.

   *Richard Levitte*

 * Improved adherence to Enhanced Security Services (ESS, RFC 2634 and RFC 5035)
   for the TSP and CMS Advanced Electronic Signatures (CAdES) implementations.
   As required by RFC 5035 check both ESSCertID and ESSCertIDv2 if both present.
   Correct the semantics of checking the validation chain in case ESSCertID{,v2}
   contains more than one certificate identifier: This means that all
   certificates referenced there MUST be part of the validation chain.

   *David von Oheimb*

 * The implementation of older EVP ciphers related to CAST, IDEA, SEED, RC2, RC4,
   RC5, DESX and DES have been moved to the legacy provider.

   *Matt Caswell*

 * The implementation of the EVP digests MD2, MD4, MDC2, WHIRLPOOL and
   RIPEMD-160 have been moved to the legacy provider.

   *Matt Caswell*

 * The deprecated function EVP_PKEY_get0() now returns NULL being called for a
   provided key.

   *Dmitry Belyavskiy*

 * The deprecated functions EVP_PKEY_get0_RSA(),
   EVP_PKEY_get0_DSA(), EVP_PKEY_get0_EC_KEY(), EVP_PKEY_get0_DH(),
   EVP_PKEY_get0_hmac(), EVP_PKEY_get0_poly1305() and EVP_PKEY_get0_siphash() as
   well as the similarly named "get1" functions behave differently in
   OpenSSL 3.0.

   *Matt Caswell*

 * A number of functions handling low-level keys or engines were deprecated
   including EVP_PKEY_set1_engine(), EVP_PKEY_get0_engine(), EVP_PKEY_assign(),
   EVP_PKEY_get0(), EVP_PKEY_get0_hmac(), EVP_PKEY_get0_poly1305() and
   EVP_PKEY_get0_siphash().

   *Matt Caswell*

 * PKCS#5 PBKDF1 key derivation has been moved from PKCS5_PBE_keyivgen() into
   the legacy crypto provider as an EVP_KDF. Applications requiring this KDF
   will need to load the legacy crypto provider. This includes these PBE
   algorithms which use this KDF:
   - NID_pbeWithMD2AndDES_CBC
   - NID_pbeWithMD5AndDES_CBC
   - NID_pbeWithSHA1AndRC2_CBC
   - NID_pbeWithMD2AndRC2_CBC
   - NID_pbeWithMD5AndRC2_CBC
   - NID_pbeWithSHA1AndDES_CBC

   *Jon Spillett*

 * Deprecated obsolete BIO_set_callback(), BIO_get_callback(), and
   BIO_debug_callback() functions.

   *Tomáš Mráz*

 * Deprecated obsolete EVP_PKEY_CTX_get0_dh_kdf_ukm() and
   EVP_PKEY_CTX_get0_ecdh_kdf_ukm() functions.

   *Tomáš Mráz*

 * The RAND_METHOD APIs have been deprecated.

   *Paul Dale*

 * The SRP APIs have been deprecated.

   *Matt Caswell*

 * Add a compile time option to prevent the caching of provider fetched
   algorithms.  This is enabled by including the no-cached-fetch option
   at configuration time.

   *Paul Dale*

 * pkcs12 now uses defaults of PBKDF2, AES and SHA-256, with a MAC iteration
   count of PKCS12_DEFAULT_ITER.

   *Tomáš Mráz and Sahana Prasad*

 * The openssl speed command does not use low-level API calls anymore.

   *Tomáš Mráz*

 * Parallel dual-prime 1024-bit modular exponentiation for AVX512_IFMA
   capable processors.

   *Ilya Albrekht, Sergey Kirillov, Andrey Matyukov (Intel Corp)*

 * Combining the Configure options no-ec and no-dh no longer disables TLSv1.3.

   *Matt Caswell*

 * Implemented support for fully "pluggable" TLSv1.3 groups. This means that
   providers may supply their own group implementations (using either the "key
   exchange" or the "key encapsulation" methods) which will automatically be
   detected and used by libssl.

   *Matt Caswell, Nicola Tuveri*

 * The undocumented function X509_certificate_type() has been deprecated;

   *Rich Salz*

 * Deprecated the obsolete BN_pseudo_rand() and BN_pseudo_rand_range().

   *Tomáš Mráz*

 * Removed RSA padding mode for SSLv23 (which was only used for
   SSLv2). This includes the functions RSA_padding_check_SSLv23() and
   RSA_padding_add_SSLv23() and the `-ssl` option in the deprecated
   `rsautl` command.

   *Rich Salz*

 * Deprecated the obsolete X9.31 RSA key generation related functions.

 * While a callback function set via `SSL_CTX_set_cert_verify_callback()`
   is not allowed to return a value > 1, this is no more taken as failure.

   *Viktor Dukhovni and David von Oheimb*

 * Deprecated the obsolete X9.31 RSA key generation related functions
   BN_X931_generate_Xpq(), BN_X931_derive_prime_ex(), and
   BN_X931_generate_prime_ex().

   *Tomáš Mráz*

 * The default key generation method for the regular 2-prime RSA keys was
   changed to the FIPS 186-4 B.3.6 method.

   *Shane Lontis*

 * Deprecated the BN_is_prime_ex() and BN_is_prime_fasttest_ex() functions.

   *Kurt Roeckx*

 * Deprecated EVP_MD_CTX_set_update_fn() and EVP_MD_CTX_update_fn().

   *Rich Salz*

 * Deprecated the type OCSP_REQ_CTX and the functions OCSP_REQ_CTX_*() and
   replaced with OSSL_HTTP_REQ_CTX and the functions OSSL_HTTP_REQ_CTX_*().

   *Rich Salz, Richard Levitte, and David von Oheimb*

 * Deprecated `X509_http_nbio()` and `X509_CRL_http_nbio()`.

   *David von Oheimb*

 * Deprecated `OCSP_parse_url()`.

   *David von Oheimb*

 * Validation of SM2 keys has been separated from the validation of regular EC
   keys.

   *Nicola Tuveri*

 * Behavior of the `pkey` app is changed, when using the `-check` or `-pubcheck`
   switches: a validation failure triggers an early exit, returning a failure
   exit status to the parent process.

   *Nicola Tuveri*

 * Changed behavior of SSL_CTX_set_ciphersuites() and SSL_set_ciphersuites()
   to ignore unknown ciphers.

   *Otto Hollmann*

 * The `-cipher-commands` and `-digest-commands` options
   of the command line utility `list` have been deprecated.
   Instead use the `-cipher-algorithms` and `-digest-algorithms` options.

   *Dmitry Belyavskiy*

 * Added convenience functions for generating asymmetric key pairs:
   The 'quick' one-shot (yet somewhat limited) function L<EVP_PKEY_Q_keygen(3)>
   and macros for the most common cases: <EVP_RSA_gen(3)> and L<EVP_EC_gen(3)>.

   *David von Oheimb*

 * All of the low level EC_KEY functions have been deprecated.

   *Shane Lontis, Paul Dale, Richard Levitte, and Tomáš Mráz*

 * Deprecated all the libcrypto and libssl error string loading
   functions.

   *Richard Levitte*

 * The functions SSL_CTX_set_tmp_dh_callback and SSL_set_tmp_dh_callback, as
   well as the macros SSL_CTX_set_tmp_dh() and SSL_set_tmp_dh() have been
   deprecated.

   *Matt Caswell*

 * The `-crypt` option to the `passwd` command line tool has been removed.

   *Paul Dale*

 * The -C option to the `x509`, `dhparam`, `dsaparam`, and `ecparam` commands
   were removed.

   *Rich Salz*

 * Add support for AES Key Wrap inverse ciphers to the EVP layer.

   *Shane Lontis*

 * Deprecated EVP_PKEY_set1_tls_encodedpoint() and
   EVP_PKEY_get1_tls_encodedpoint().

   *Matt Caswell*

 * The security callback, which can be customised by application code, supports
   the security operation SSL_SECOP_TMP_DH. One location of the "other" parameter
   was incorrectly passing a DH object. It now passed an EVP_PKEY in all cases.

   *Matt Caswell*

 * Add PKCS7_get_octet_string() and PKCS7_type_is_other() to the public
   interface. Their functionality remains unchanged.

   *Jordan Montgomery*

 * Added new option for 'openssl list', '-providers', which will display the
   list of loaded providers, their names, version and status.  It optionally
   displays their gettable parameters.

   *Paul Dale*

 * Removed EVP_PKEY_set_alias_type().

   *Richard Levitte*

 * Deprecated `EVP_PKEY_CTX_set_rsa_keygen_pubexp()` and introduced
   `EVP_PKEY_CTX_set1_rsa_keygen_pubexp()`, which is now preferred.

   *Jeremy Walch*

 * Changed all "STACK" functions to be macros instead of inline functions. Macro
   parameters are still checked for type safety at compile time via helper
   inline functions.

   *Matt Caswell*

 * Remove the RAND_DRBG API

   *Paul Dale and Matthias St. Pierre*

 * Allow `SSL_set1_host()` and `SSL_add1_host()` to take IP literal addresses
   as well as actual hostnames.

   *David Woodhouse*

 * The 'MinProtocol' and 'MaxProtocol' configuration commands now silently
   ignore TLS protocol version bounds when configuring DTLS-based contexts, and
   conversely, silently ignore DTLS protocol version bounds when configuring
   TLS-based contexts.  The commands can be repeated to set bounds of both
   types.  The same applies with the corresponding "min_protocol" and
   "max_protocol" command-line switches, in case some application uses both TLS
   and DTLS.

   SSL_CTX instances that are created for a fixed protocol version (e.g.
   `TLSv1_server_method()`) also silently ignore version bounds.  Previously
   attempts to apply bounds to these protocol versions would result in an
   error.  Now only the "version-flexible" SSL_CTX instances are subject to
   limits in configuration files in command-line options.

   *Viktor Dukhovni*

 * Deprecated the `ENGINE` API.  Engines should be replaced with providers
   going forward.

   *Paul Dale*

 * Reworked the recorded ERR codes to make better space for system errors.
   To distinguish them, the macro `ERR_SYSTEM_ERROR()` indicates if the
   given code is a system error (true) or an OpenSSL error (false).

   *Richard Levitte*

 * Reworked the test perl framework to better allow parallel testing.

   *Nicola Tuveri and David von Oheimb*

 * Added ciphertext stealing algorithms AES-128-CBC-CTS, AES-192-CBC-CTS and
   AES-256-CBC-CTS to the providers. CS1, CS2 and CS3 variants are supported.

   *Shane Lontis*

 * 'Configure' has been changed to figure out the configuration target if
   none is given on the command line.  Consequently, the 'config' script is
   now only a mere wrapper.  All documentation is changed to only mention
   'Configure'.

   *Rich Salz and Richard Levitte*

 * Added a library context `OSSL_LIB_CTX` that applications as well as
   other libraries can use to form a separate context within which
   libcrypto operations are performed.

   *Richard Levitte*

 * Added various `_ex` functions to the OpenSSL API that support using
   a non-default `OSSL_LIB_CTX`.

   *OpenSSL team*

 * Handshake now fails if Extended Master Secret extension is dropped
   on renegotiation.

   *Tomáš Mráz*

 * Dropped interactive mode from the `openssl` program.

   *Richard Levitte*

 * Deprecated `EVP_PKEY_cmp()` and `EVP_PKEY_cmp_parameters()`.

   *David von Oheimb and Shane Lontis*

 * Deprecated `EC_METHOD_get_field_type()`.

   *Billy Bob Brumley*

 * Deprecated EC_GFp_simple_method(), EC_GFp_mont_method(),
   EC_GF2m_simple_method(), EC_GFp_nist_method(), EC_GFp_nistp224_method()
   EC_GFp_nistp256_method(), and EC_GFp_nistp521_method().

   *Billy Bob Brumley*

 * Deprecated EC_GROUP_new(), EC_GROUP_method_of(), and EC_POINT_method_of().

   *Billy Bob Brumley*

 * Add CAdES-BES signature verification support, mostly derived
   from ESSCertIDv2 TS (RFC 5816) contribution by Marek Klein.

   *Filipe Raimundo da Silva*

 * Add CAdES-BES signature scheme and attributes support (RFC 5126) to CMS API.

   *Antonio Iacono*

 * Added the AuthEnvelopedData content type structure (RFC 5083) with AES-GCM
   parameter (RFC 5084) for the Cryptographic Message Syntax (CMS).

   *Jakub Zelenka*

 * Deprecated EC_POINT_make_affine() and EC_POINTs_make_affine().

   *Billy Bob Brumley*

 * Deprecated EC_GROUP_precompute_mult(), EC_GROUP_have_precompute_mult(), and
   EC_KEY_precompute_mult().

   *Billy Bob Brumley*

 * Deprecated EC_POINTs_mul().

   *Billy Bob Brumley*

 * Removed FIPS_mode() and FIPS_mode_set().

   *Shane Lontis*

 * The SSL option SSL_OP_IGNORE_UNEXPECTED_EOF is introduced.

   *Dmitry Belyavskiy*

 * Deprecated EC_POINT_set_Jprojective_coordinates_GFp() and
   EC_POINT_get_Jprojective_coordinates_GFp().

   *Billy Bob Brumley*

 * Added OSSL_PARAM_BLD to the public interface.  This allows OSSL_PARAM
   arrays to be more easily constructed via a series of utility functions.
   Create a parameter builder using OSSL_PARAM_BLD_new(), add parameters using
   the various push functions and finally convert to a passable OSSL_PARAM
   array using OSSL_PARAM_BLD_to_param().

   *Paul Dale*

 * The security strength of SHA1 and MD5 based signatures in TLS has been
   reduced.

   *Kurt Roeckx*

 * Added EVP_PKEY_set_type_by_keymgmt(), to initialise an EVP_PKEY to
   contain a provider side internal key.

   *Richard Levitte*

 * ASN1_verify(), ASN1_digest() and ASN1_sign() have been deprecated.

   *Richard Levitte*

 * Project text documents not yet having a proper file name extension
   (`HACKING`, `LICENSE`, `NOTES*`, `README*`, `VERSION`) have been renamed to
   `*.md` as far as reasonable, else `*.txt`, for better use with file managers.

   *David von Oheimb*

 * The main project documents (README, NEWS, CHANGES, INSTALL, SUPPORT)
   have been converted to Markdown with the goal to produce documents
   which not only look pretty when viewed online in the browser, but
   remain well readable inside a plain text editor.

   To achieve this goal, a 'minimalistic' Markdown style has been applied
   which avoids formatting elements that interfere too much with the
   reading flow in the text file. For example, it

   * avoids [ATX headings][] and uses [setext headings][] instead
     (which works for `<h1>` and `<h2>` headings only).
   * avoids [inline links][] and uses [reference links][] instead.
   * avoids [fenced code blocks][] and uses [indented code blocks][] instead.

     [ATX headings]:         https://github.github.com/gfm/#atx-headings
     [setext headings]:      https://github.github.com/gfm/#setext-headings
     [inline links]:         https://github.github.com/gfm/#inline-link
     [reference links]:      https://github.github.com/gfm/#reference-link
     [fenced code blocks]:   https://github.github.com/gfm/#fenced-code-blocks
     [indented code blocks]: https://github.github.com/gfm/#indented-code-blocks

   *Matthias St. Pierre*

 * The test suite is changed to preserve results of each test recipe.
   A new directory test-runs/ with subdirectories named like the
   test recipes are created in the build tree for this purpose.

   *Richard Levitte*

 * Added an implementation of CMP and CRMF (RFC 4210, RFC 4211 RFC 6712).
   This adds `crypto/cmp/`, `crpyto/crmf/`, `apps/cmp.c`, and `test/cmp_*`.
   See L<openssl-cmp(1)> and L<OSSL_CMP_exec_IR_ses(3)> as starting points.

   *David von Oheimb, Martin Peylo*

 * Generalized the HTTP client code from `crypto/ocsp/` into `crpyto/http/`.
   It supports arbitrary request and response content types, GET redirection,
   TLS, connections via HTTP(S) proxies, connections and exchange via
   user-defined BIOs (allowing implicit connections), persistent connections,
   and timeout checks.  See L<OSSL_HTTP_transfer(3)> etc. for details.
   The legacy OCSP-focused (and only partly documented) API
   is retained for backward compatibility, while most of it is deprecated.

   *David von Oheimb*

 * Added `util/check-format.pl`, a tool for checking adherence to the
   OpenSSL coding style <https://www.openssl.org/policies/codingstyle.html>.
   The checks performed are incomplete and yield some false positives.
   Still the tool should be useful for detecting most typical glitches.

   *David von Oheimb*

 * `BIO_do_connect()` and `BIO_do_handshake()` have been extended:
   If domain name resolution yields multiple IP addresses all of them are tried
   after `connect()` failures.

   *David von Oheimb*

 * All of the low level RSA functions have been deprecated.

   *Paul Dale*

 * X509 certificates signed using SHA1 are no longer allowed at security
   level 1 and above.

   *Kurt Roeckx*

 * The command line utilities dhparam, dsa, gendsa and dsaparam have been
   modified to use PKEY APIs.  These commands are now in maintenance mode
   and no new features will be added to them.

   *Paul Dale*

 * The command line utility rsautl has been deprecated.

   *Paul Dale*

 * The command line utilities genrsa and rsa have been modified to use PKEY
   APIs. They now write PKCS#8 keys by default. These commands are now in
   maintenance mode and no new features will be added to them.

   *Paul Dale*

 * All of the low level DH functions have been deprecated.

   *Paul Dale and Matt Caswell*

 * All of the low level DSA functions have been deprecated.

   *Paul Dale*

 * Reworked the treatment of EC EVP_PKEYs with the SM2 curve to
   automatically become EVP_PKEY_SM2 rather than EVP_PKEY_EC.

   *Richard Levitte*

 * Deprecated low level ECDH and ECDSA functions.

   *Paul Dale*

 * Deprecated EVP_PKEY_decrypt_old() and EVP_PKEY_encrypt_old().

   *Richard Levitte*

 * Enhanced the documentation of EVP_PKEY_get_size(), EVP_PKEY_get_bits()
   and EVP_PKEY_get_security_bits().  Especially EVP_PKEY_get_size() needed
   a new formulation to include all the things it can be used for,
   as well as words of caution.

   *Richard Levitte*

 * The SSL_CTX_set_tlsext_ticket_key_cb(3) function has been deprecated.

   *Paul Dale*

 * All of the low level HMAC functions have been deprecated.

   *Paul Dale and David von Oheimb*

 * Over two thousand fixes were made to the documentation, including:
   - Common options (such as -rand/-writerand, TLS version control, etc)
     were refactored and point to newly-enhanced descriptions in openssl.pod.
   - Added style conformance for all options (with help from Richard Levitte),
     documented all reported missing options, added a CI build to check
     that all options are documented and that no unimplemented options
     are documented.
   - Documented some internals, such as all use of environment variables.
   - Addressed all internal broken L<> references.

   *Rich Salz*

 * All of the low level CMAC functions have been deprecated.

   *Paul Dale*

 * The low-level MD2, MD4, MD5, MDC2, RIPEMD160 and Whirlpool digest
   functions have been deprecated.

   *Paul Dale and David von Oheimb*

 * Corrected the documentation of the return values from the `EVP_DigestSign*`
   set of functions.  The documentation mentioned negative values for some
   errors, but this was never the case, so the mention of negative values
   was removed.

   Code that followed the documentation and thereby check with something
   like `EVP_DigestSignInit(...) <= 0` will continue to work undisturbed.

   *Richard Levitte*

 * All of the low level cipher functions have been deprecated.

   *Matt Caswell and Paul Dale*

 * Removed include/openssl/opensslconf.h.in and replaced it with
   include/openssl/configuration.h.in, which differs in not including
   <openssl/macros.h>.  A short header include/openssl/opensslconf.h
   was added to include both.

   This allows internal hacks where one might need to modify the set
   of configured macros, for example this if deprecated symbols are
   still supposed to be available internally:

       #include <openssl/configuration.h>

       #undef OPENSSL_NO_DEPRECATED
       #define OPENSSL_SUPPRESS_DEPRECATED

       #include <openssl/macros.h>

   This should not be used by applications that use the exported
   symbols, as that will lead to linking errors.

   *Richard Levitte*

 * Fixed an overflow bug in the x64_64 Montgomery squaring procedure
   used in exponentiation with 512-bit moduli. No EC algorithms are
   affected. Analysis suggests that attacks against 2-prime RSA1024,
   3-prime RSA1536, and DSA1024 as a result of this defect would be very
   difficult to perform and are not believed likely. Attacks against DH512
   are considered just feasible. However, for an attack the target would
   have to re-use the DH512 private key, which is not recommended anyway.
   Also applications directly using the low-level API BN_mod_exp may be
   affected if they use BN_FLG_CONSTTIME.
   ([CVE-2019-1551])

   *Andy Polyakov*

 * Most memory-debug features have been deprecated, and the functionality
   replaced with no-ops.

   *Rich Salz*

 * Added documentation for the STACK API.

   *Rich Salz*

 * Introduced a new method type and API, OSSL_ENCODER, to represent
   generic encoders.  These do the same sort of job that PEM writers
   and d2i functions do, but with support for methods supplied by
   providers, and the possibility for providers to support other
   formats as well.

   *Richard Levitte*

 * Introduced a new method type and API, OSSL_DECODER, to represent
   generic decoders.  These do the same sort of job that PEM readers
   and i2d functions do, but with support for methods supplied by
   providers, and the possibility for providers to support other
   formats as well.

   *Richard Levitte*

 * Added a .pragma directive to the syntax of configuration files, to
   allow varying behavior in a supported and predictable manner.
   Currently added pragma:

           .pragma dollarid:on

   This allows dollar signs to be a keyword character unless it's
   followed by a opening brace or parenthesis.  This is useful for
   platforms where dollar signs are commonly used in names, such as
   volume names and system directory names on VMS.

   *Richard Levitte*

 * Added functionality to create an EVP_PKEY from user data.

   *Richard Levitte*

 * Change the interpretation of the '--api' configuration option to
   mean that this is a desired API compatibility level with no
   further meaning.  The previous interpretation, that this would
   also mean to remove all deprecated symbols up to and including
   the given version, no requires that 'no-deprecated' is also used
   in the configuration.

   When building applications, the desired API compatibility level
   can be set with the OPENSSL_API_COMPAT macro like before.  For
   API compatibility version below 3.0, the old style numerical
   value is valid as before, such as -DOPENSSL_API_COMPAT=0x10100000L.
   For version 3.0 and on, the value is expected to be the decimal
   value calculated from the major and minor version like this:

           MAJOR * 10000 + MINOR * 100

   Examples:

           -DOPENSSL_API_COMPAT=30000             For 3.0
           -DOPENSSL_API_COMPAT=30200             For 3.2

   To hide declarations that are deprecated up to and including the
   given API compatibility level, -DOPENSSL_NO_DEPRECATED must be
   given when building the application as well.

   *Richard Levitte*

 * Added the X509_LOOKUP_METHOD called X509_LOOKUP_store, to allow
   access to certificate and CRL stores via URIs and OSSL_STORE
   loaders.

   This adds the following functions:

   - X509_LOOKUP_store()
   - X509_STORE_load_file()
   - X509_STORE_load_path()
   - X509_STORE_load_store()
   - SSL_add_store_cert_subjects_to_stack()
   - SSL_CTX_set_default_verify_store()
   - SSL_CTX_load_verify_file()
   - SSL_CTX_load_verify_dir()
   - SSL_CTX_load_verify_store()

   *Richard Levitte*

 * Added a new method to gather entropy on VMS, based on SYS$GET_ENTROPY.
   The presence of this system service is determined at run-time.

   *Richard Levitte*

 * Added functionality to create an EVP_PKEY context based on data
   for methods from providers.  This takes an algorithm name and a
   property query string and simply stores them, with the intent
   that any operation that uses this context will use those strings
   to fetch the needed methods implicitly, thereby making the port
   of application written for pre-3.0 OpenSSL easier.

   *Richard Levitte*

 * The undocumented function NCONF_WIN32() has been deprecated; for
   conversion details see the HISTORY section of doc/man5/config.pod

   *Rich Salz*

 * Introduced the new functions EVP_DigestSignInit_ex() and
   EVP_DigestVerifyInit_ex(). The macros EVP_DigestSignUpdate() and
   EVP_DigestVerifyUpdate() have been converted to functions. See the man
   pages for further details.

   *Matt Caswell*

 * Over two thousand fixes were made to the documentation, including:
   adding missing command flags, better style conformance, documentation
   of internals, etc.

   *Rich Salz, Richard Levitte*

 * s390x assembly pack: add hardware-support for P-256, P-384, P-521,
   X25519, X448, Ed25519 and Ed448.

   *Patrick Steuer*

 * Print all values for a PKCS#12 attribute with 'openssl pkcs12', not just
   the first value.

   *Jon Spillett*

 * Deprecated the public definition of `ERR_STATE` as well as the function
   `ERR_get_state()`.  This is done in preparation of making `ERR_STATE` an
   opaque type.

   *Richard Levitte*

 * Added ERR functionality to give callers access to the stored function
   names that have replaced the older function code based functions.

   New functions are ERR_peek_error_func(), ERR_peek_last_error_func(),
   ERR_peek_error_data(), ERR_peek_last_error_data(), ERR_get_error_all(),
   ERR_peek_error_all() and ERR_peek_last_error_all().

   Deprecate ERR functions ERR_get_error_line(), ERR_get_error_line_data(),
   ERR_peek_error_line_data(), ERR_peek_last_error_line_data() and
   ERR_func_error_string().

   *Richard Levitte*

 * Extended testing to be verbose for failing tests only.  The make variables
   VERBOSE_FAILURE or VF can be used to enable this:

           $ make VF=1 test                           # Unix
           $ mms /macro=(VF=1) test                   ! OpenVMS
           $ nmake VF=1 test                          # Windows

   *Richard Levitte*

 * Added the `-copy_extensions` option to the `x509` command for use with
   `-req` and `-x509toreq`. When given with the `copy` or `copyall` argument,
   all extensions in the request are copied to the certificate or vice versa.

   *David von Oheimb*, *Kirill Stefanenkov <kirill_stefanenkov@rambler.ru>*

 * Added the `-copy_extensions` option to the `req` command for use with
   `-x509`. When given with the `copy` or `copyall` argument,
   all extensions in the certification request are copied to the certificate.

   *David von Oheimb*

 * The `x509`, `req`, and `ca` commands now make sure that X.509v3 certificates
   they generate are by default RFC 5280 compliant in the following sense:
   There is a subjectKeyIdentifier extension with a hash value of the public key
   and for not self-signed certs there is an authorityKeyIdentifier extension
   with a keyIdentifier field or issuer information identifying the signing key.
   This is done unless some configuration overrides the new default behavior,
   such as `subjectKeyIdentifier = none` and `authorityKeyIdentifier = none`.

   *David von Oheimb*

 * Added several checks to `X509_verify_cert()` according to requirements in
   RFC 5280 in case `X509_V_FLAG_X509_STRICT` is set
   (which may be done by using the CLI option `-x509_strict`):
   * The basicConstraints of CA certificates must be marked critical.
   * CA certificates must explicitly include the keyUsage extension.
   * If a pathlenConstraint is given the key usage keyCertSign must be allowed.
   * The issuer name of any certificate must not be empty.
   * The subject name of CA certs, certs with keyUsage crlSign,
     and certs without subjectAlternativeName must not be empty.
   * If a subjectAlternativeName extension is given it must not be empty.
   * The signatureAlgorithm field and the cert signature must be consistent.
   * Any given authorityKeyIdentifier and any given subjectKeyIdentifier
     must not be marked critical.
   * The authorityKeyIdentifier must be given for X.509v3 certs
     unless they are self-signed.
   * The subjectKeyIdentifier must be given for all X.509v3 CA certs.

   *David von Oheimb*

 * Certificate verification using `X509_verify_cert()` meanwhile rejects EC keys
   with explicit curve parameters (specifiedCurve) as required by RFC 5480.

   *Tomáš Mráz*

 * For built-in EC curves, ensure an EC_GROUP built from the curve name is
   used even when parsing explicit parameters, when loading a encoded key
   or calling `EC_GROUP_new_from_ecpkparameters()`/
   `EC_GROUP_new_from_ecparameters()`.
   This prevents bypass of security hardening and performance gains,
   especially for curves with specialized EC_METHODs.
   By default, if a key encoded with explicit parameters is loaded and later
   encoded, the output is still encoded with explicit parameters, even if
   internally a "named" EC_GROUP is used for computation.

   *Nicola Tuveri*

 * Compute ECC cofactors if not provided during EC_GROUP construction. Before
   this change, EC_GROUP_set_generator would accept order and/or cofactor as
   NULL. After this change, only the cofactor parameter can be NULL. It also
   does some minimal sanity checks on the passed order.
   ([CVE-2019-1547])

   *Billy Bob Brumley*

 * Fixed a padding oracle in PKCS7_dataDecode and CMS_decrypt_set1_pkey.
   An attack is simple, if the first CMS_recipientInfo is valid but the
   second CMS_recipientInfo is chosen ciphertext. If the second
   recipientInfo decodes to PKCS #1 v1.5 form plaintext, the correct
   encryption key will be replaced by garbage, and the message cannot be
   decoded, but if the RSA decryption fails, the correct encryption key is
   used and the recipient will not notice the attack.
   As a work around for this potential attack the length of the decrypted
   key must be equal to the cipher default key length, in case the
   certifiate is not given and all recipientInfo are tried out.
   The old behaviour can be re-enabled in the CMS code by setting the
   CMS_DEBUG_DECRYPT flag.

   *Bernd Edlinger*

 * Early start up entropy quality from the DEVRANDOM seed source has been
   improved for older Linux systems.  The RAND subsystem will wait for
   /dev/random to be producing output before seeding from /dev/urandom.
   The seeded state is stored for future library initialisations using
   a system global shared memory segment.  The shared memory identifier
   can be configured by defining OPENSSL_RAND_SEED_DEVRANDOM_SHM_ID to
   the desired value.  The default identifier is 114.

   *Paul Dale*

 * Revised BN_generate_prime_ex to not avoid factors 2..17863 in p-1
   when primes for RSA keys are computed.
   Since we previously always generated primes == 2 (mod 3) for RSA keys,
   the 2-prime and 3-prime RSA modules were easy to distinguish, since
   `N = p*q = 1 (mod 3)`, but `N = p*q*r = 2 (mod 3)`. Therefore fingerprinting
   2-prime vs. 3-prime RSA keys was possible by computing N mod 3.
   This avoids possible fingerprinting of newly generated RSA modules.

   *Bernd Edlinger*

 * Correct the extended master secret constant on EBCDIC systems. Without this
   fix TLS connections between an EBCDIC system and a non-EBCDIC system that
   negotiate EMS will fail. Unfortunately this also means that TLS connections
   between EBCDIC systems with this fix, and EBCDIC systems without this
   fix will fail if they negotiate EMS.

   *Matt Caswell*

 * Changed the library initialisation so that the config file is now loaded
   by default. This was already the case for libssl. It now occurs for both
   libcrypto and libssl. Use the OPENSSL_INIT_NO_LOAD_CONFIG option to
   `OPENSSL_init_crypto()` to suppress automatic loading of a config file.

   *Matt Caswell*

 * Introduced new error raising macros, `ERR_raise()` and `ERR_raise_data()`,
   where the former acts as a replacement for `ERR_put_error()`, and the
   latter replaces the combination `ERR_put_error()` + `ERR_add_error_data()`.
   `ERR_raise_data()` adds more flexibility by taking a format string and
   an arbitrary number of arguments following it, to be processed with
   `BIO_snprintf()`.

   *Richard Levitte*

 * Introduced a new function, `OSSL_PROVIDER_available()`, which can be used
   to check if a named provider is loaded and available.  When called, it
   will also activate all fallback providers if such are still present.

   *Richard Levitte*

 * Enforce a minimum DH modulus size of 512 bits.

   *Bernd Edlinger*

 * Changed DH parameters to generate the order q subgroup instead of 2q.
   Previously generated DH parameters are still accepted by DH_check
   but DH_generate_key works around that by clearing bit 0 of the
   private key for those. This avoids leaking bit 0 of the private key.

   *Bernd Edlinger*

 * Significantly reduce secure memory usage by the randomness pools.

   *Paul Dale*

 * `{CRYPTO,OPENSSL}_mem_debug_{push,pop}` are now no-ops and have been
   deprecated.

   *Rich Salz*

 * A new type, EVP_KEYEXCH, has been introduced to represent key exchange
   algorithms. An implementation of a key exchange algorithm can be obtained
   by using the function EVP_KEYEXCH_fetch(). An EVP_KEYEXCH algorithm can be
   used in a call to EVP_PKEY_derive_init_ex() which works in a similar way to
   the older EVP_PKEY_derive_init() function. See the man pages for the new
   functions for further details.

   *Matt Caswell*

 * The EVP_PKEY_CTX_set_dh_pad() macro has now been converted to a function.

   *Matt Caswell*

 * Removed the function names from error messages and deprecated the
   xxx_F_xxx define's.

   *Richard Levitte*

 * Removed NextStep support and the macro OPENSSL_UNISTD

   *Rich Salz*

 * Removed DES_check_key.  Also removed OPENSSL_IMPLEMENT_GLOBAL,
   OPENSSL_GLOBAL_REF, OPENSSL_DECLARE_GLOBAL.
   Also removed "export var as function" capability; we do not export
   variables, only functions.

   *Rich Salz*

 * RC5_32_set_key has been changed to return an int type, with 0 indicating
   an error and 1 indicating success. In previous versions of OpenSSL this
   was a void type. If a key was set longer than the maximum possible this
   would crash.

   *Matt Caswell*

 * Support SM2 signing and verification schemes with X509 certificate.

   *Paul Yang*

 * Use SHA256 as the default digest for TS query in the `ts` app.

   *Tomáš Mráz*

 * Change PBKDF2 to conform to SP800-132 instead of the older PKCS5 RFC2898.

   *Shane Lontis*

 * Default cipher lists/suites are now available via a function, the
   #defines are deprecated.

   *Todd Short*

 * Add target VC-WIN32-UWP, VC-WIN64A-UWP, VC-WIN32-ARM-UWP and
   VC-WIN64-ARM-UWP in Windows OneCore target for making building libraries
   for Windows Store apps easier. Also, the "no-uplink" option has been added.

   *Kenji Mouri*

 * Join the directories crypto/x509 and crypto/x509v3

   *Richard Levitte*

 * Added command 'openssl kdf' that uses the EVP_KDF API.

   *Shane Lontis*

 * Added command 'openssl mac' that uses the EVP_MAC API.

   *Shane Lontis*

 * Added OPENSSL_info() to get diverse built-in OpenSSL data, such
   as default directories.  Also added the command 'openssl info'
   for scripting purposes.

   *Richard Levitte*

 * The functions AES_ige_encrypt() and AES_bi_ige_encrypt() have been
   deprecated.

   *Matt Caswell*

 * Add prediction resistance to the DRBG reseeding process.

   *Paul Dale*

 * Limit the number of blocks in a data unit for AES-XTS to 2^20 as
   mandated by IEEE Std 1619-2018.

   *Paul Dale*

 * Added newline escaping functionality to a filename when using openssl dgst.
   This output format is to replicate the output format found in the `*sum`
   checksum programs. This aims to preserve backward compatibility.

   *Matt Eaton, Richard Levitte, and Paul Dale*

 * Removed the heartbeat message in DTLS feature, as it has very
   little usage and doesn't seem to fulfill a valuable purpose.
   The configuration option is now deprecated.

   *Richard Levitte*

 * Changed the output of 'openssl {digestname} < file' to display the
   digest name in its output.

   *Richard Levitte*

 * Added a new generic trace API which provides support for enabling
   instrumentation through trace output.

   *Richard Levitte & Matthias St. Pierre*

 * Added build tests for C++.  These are generated files that only do one
   thing, to include one public OpenSSL head file each.  This tests that
   the public header files can be usefully included in a C++ application.

   This test isn't enabled by default.  It can be enabled with the option
   'enable-buildtest-c++'.

   *Richard Levitte*

 * Added KB KDF (EVP_KDF_KB) to EVP_KDF.

   *Robbie Harwood*

 * Added SSH KDF (EVP_KDF_SSHKDF) and KRB5 KDF (EVP_KDF_KRB5KDF) to EVP_KDF.

   *Simo Sorce*

 * Added Single Step KDF (EVP_KDF_SS), X963 KDF, and X942 KDF to EVP_KDF.

   *Shane Lontis*

 * Added KMAC to EVP_MAC.

   *Shane Lontis*

 * Added property based algorithm implementation selection framework to
   the core.

   *Paul Dale*

 * Added SCA hardening for modular field inversion in EC_GROUP through
   a new dedicated field_inv() pointer in EC_METHOD.
   This also addresses a leakage affecting conversions from projective
   to affine coordinates.

   *Billy Bob Brumley, Nicola Tuveri*

 * Added EVP_KDF, an EVP layer KDF API, to simplify adding KDF and PRF
   implementations.  This includes an EVP_PKEY to EVP_KDF bridge for
   those algorithms that were already supported through the EVP_PKEY API
   (scrypt, TLS1 PRF and HKDF).  The low-level KDF functions for PBKDF2
   and scrypt are now wrappers that call EVP_KDF.

   *David Makepeace*

 * Build devcrypto engine as a dynamic engine.

   *Eneas U de Queiroz*

 * Add keyed BLAKE2 to EVP_MAC.

   *Antoine Salon*

 * Fix a bug in the computation of the endpoint-pair shared secret used
   by DTLS over SCTP. This breaks interoperability with older versions
   of OpenSSL like OpenSSL 1.1.0 and OpenSSL 1.0.2. There is a runtime
   switch SSL_MODE_DTLS_SCTP_LABEL_LENGTH_BUG (off by default) enabling
   interoperability with such broken implementations. However, enabling
   this switch breaks interoperability with correct implementations.

 * Fix a use after free bug in d2i_X509_PUBKEY when overwriting a
   re-used X509_PUBKEY object if the second PUBKEY is malformed.

   *Bernd Edlinger*

 * Move strictness check from EVP_PKEY_asn1_new() to EVP_PKEY_asn1_add0().

   *Richard Levitte*

 * Changed the license to the Apache License v2.0.

   *Richard Levitte*

 * Switch to a new version scheme using three numbers MAJOR.MINOR.PATCH.

   - Major releases (indicated by incrementing the MAJOR release number)
     may introduce incompatible API/ABI changes.
   - Minor releases (indicated by incrementing the MINOR release number)
     may introduce new features but retain API/ABI compatibility.
   - Patch releases (indicated by incrementing the PATCH number)
     are intended for bug fixes and other improvements of existing
     features only (like improving performance or adding documentation)
     and retain API/ABI compatibility.

   *Richard Levitte*

 * Add support for RFC5297 SIV mode (siv128), including AES-SIV.

   *Todd Short*

 * Remove the 'dist' target and add a tarball building script.  The
   'dist' target has fallen out of use, and it shouldn't be
   necessary to configure just to create a source distribution.

   *Richard Levitte*

 * Recreate the OS390-Unix config target.  It no longer relies on a
   special script like it did for OpenSSL pre-1.1.0.

   *Richard Levitte*

 * Instead of having the source directories listed in Configure, add
   a 'build.info' keyword SUBDIRS to indicate what sub-directories to
   look into.

   *Richard Levitte*

 * Add GMAC to EVP_MAC.

   *Paul Dale*

 * Ported the HMAC, CMAC and SipHash EVP_PKEY_METHODs to EVP_MAC.

   *Richard Levitte*

 * Added EVP_MAC, an EVP layer MAC API, to simplify adding MAC
   implementations.  This includes a generic EVP_PKEY to EVP_MAC bridge,
   to facilitate the continued use of MACs through raw private keys in
   functionality such as `EVP_DigestSign*` and `EVP_DigestVerify*`.

   *Richard Levitte*

 * Deprecate ECDH_KDF_X9_62().

   *Antoine Salon*

 * Added EVP_PKEY_ECDH_KDF_X9_63 and ecdh_KDF_X9_63() as replacements for
   the EVP_PKEY_ECDH_KDF_X9_62 KDF type and ECDH_KDF_X9_62(). The old names
   are retained for backwards compatibility.

   *Antoine Salon*

 * AES-XTS mode now enforces that its two keys are different to mitigate
   the attacked described in "Efficient Instantiations of Tweakable
   Blockciphers and Refinements to Modes OCB and PMAC" by Phillip Rogaway.
   Details of this attack can be obtained from:
   <http://web.cs.ucdavis.edu/%7Erogaway/papers/offsets.pdf>

   *Paul Dale*

 * Rename the object files, i.e. give them other names than in previous
   versions.  Their names now include the name of the final product, as
   well as its type mnemonic (bin, lib, shlib).

   *Richard Levitte*

 * Added new option for 'openssl list', '-objects', which will display the
   list of built in objects, i.e. OIDs with names.

   *Richard Levitte*

 * Added the options `-crl_lastupdate` and `-crl_nextupdate` to `openssl ca`,
   allowing the `lastUpdate` and `nextUpdate` fields in the generated CRL to
   be set explicitly.

   *Chris Novakovic*

 * Added support for Linux Kernel TLS data-path. The Linux Kernel data-path
   improves application performance by removing data copies and providing
   applications with zero-copy system calls such as sendfile and splice.

   *Boris Pismenny*

 * The SSL option SSL_OP_CLEANSE_PLAINTEXT is introduced.

   *Martin Elshuber*

 * `PKCS12_parse` now maintains the order of the parsed certificates
   when outputting them via `*ca` (rather than reversing it).

   *David von Oheimb*

 * Deprecated pthread fork support methods.

   *Randall S. Becker*

 * Added support for FFDHE key exchange in TLS 1.3.

   *Raja Ashok*

 * Added a new concept for OpenSSL plugability: providers.  This
   functionality is designed to replace the ENGINE API and ENGINE
   implementations, and to be much more dynamic, allowing provider
   authors to introduce new algorithms among other things, as long as
   there's an API that supports the algorithm type.

   With this concept comes a new core API for interaction between
   libcrypto and provider implementations.  Public libcrypto functions
   that want to use providers do so through this core API.

   The main documentation for this core API is found in
   doc/man7/provider.pod, doc/man7/provider-base.pod, and they in turn
   refer to other manuals describing the API specific for supported
   algorithm types (also called operations).

   *The OpenSSL team*

OpenSSL 1.1.1
-------------

### Changes between 1.1.1l and 1.1.1m [xx XXX xxxx]

 * Avoid loading of a dynamic engine twice.

   *Bernd Edlinger*

 * Prioritise DANE TLSA issuer certs over peer certs

   *Viktor Dukhovni*

 * Fixed random API for MacOS prior to 10.12

   These MacOS versions don't support the CommonCrypto APIs

   *Lenny Primak*

### Changes between 1.1.1k and 1.1.1l [24 Aug 2021]

 * Fixed an SM2 Decryption Buffer Overflow.

   In order to decrypt SM2 encrypted data an application is expected to
   call the API function EVP_PKEY_decrypt(). Typically an application will
   call this function twice. The first time, on entry, the "out" parameter
   can be NULL and, on exit, the "outlen" parameter is populated with the
   buffer size required to hold the decrypted plaintext. The application
   can then allocate a sufficiently sized buffer and call EVP_PKEY_decrypt()
   again, but this time passing a non-NULL value for the "out" parameter.

   A bug in the implementation of the SM2 decryption code means that the
   calculation of the buffer size required to hold the plaintext returned
   by the first call to EVP_PKEY_decrypt() can be smaller than the actual
   size required by the second call. This can lead to a buffer overflow
   when EVP_PKEY_decrypt() is called by the application a second time with
   a buffer that is too small.

   A malicious attacker who is able present SM2 content for decryption to
   an application could cause attacker chosen data to overflow the buffer
   by up to a maximum of 62 bytes altering the contents of other data held
   after the buffer, possibly changing application behaviour or causing
   the application to crash. The location of the buffer is application
   dependent but is typically heap allocated.
   ([CVE-2021-3711])

   *Matt Caswell*

 * Fixed various read buffer overruns processing ASN.1 strings

   ASN.1 strings are represented internally within OpenSSL as an ASN1_STRING
   structure which contains a buffer holding the string data and a field
   holding the buffer length. This contrasts with normal C strings which
   are repesented as a buffer for the string data which is terminated
   with a NUL (0) byte.

   Although not a strict requirement, ASN.1 strings that are parsed using
   OpenSSL's own "d2i" functions (and other similar parsing functions) as
   well as any string whose value has been set with the ASN1_STRING_set()
   function will additionally NUL terminate the byte array in the
   ASN1_STRING structure.

   However, it is possible for applications to directly construct valid
   ASN1_STRING structures which do not NUL terminate the byte array by
   directly setting the "data" and "length" fields in the ASN1_STRING
   array. This can also happen by using the ASN1_STRING_set0() function.

   Numerous OpenSSL functions that print ASN.1 data have been found to
   assume that the ASN1_STRING byte array will be NUL terminated, even
   though this is not guaranteed for strings that have been directly
   constructed. Where an application requests an ASN.1 structure to be
   printed, and where that ASN.1 structure contains ASN1_STRINGs that have
   been directly constructed by the application without NUL terminating
   the "data" field, then a read buffer overrun can occur.

   The same thing can also occur during name constraints processing
   of certificates (for example if a certificate has been directly
   constructed by the application instead of loading it via the OpenSSL
   parsing functions, and the certificate contains non NUL terminated
   ASN1_STRING structures). It can also occur in the X509_get1_email(),
   X509_REQ_get1_email() and X509_get1_ocsp() functions.

   If a malicious actor can cause an application to directly construct an
   ASN1_STRING and then process it through one of the affected OpenSSL
   functions then this issue could be hit. This might result in a crash
   (causing a Denial of Service attack). It could also result in the
   disclosure of private memory contents (such as private keys, or
   sensitive plaintext).
   ([CVE-2021-3712])

   *Matt Caswell*

### Changes between 1.1.1j and 1.1.1k [25 Mar 2021]

 * Fixed a problem with verifying a certificate chain when using the
   X509_V_FLAG_X509_STRICT flag. This flag enables additional security checks of
   the certificates present in a certificate chain. It is not set by default.

   Starting from OpenSSL version 1.1.1h a check to disallow certificates in
   the chain that have explicitly encoded elliptic curve parameters was added
   as an additional strict check.

   An error in the implementation of this check meant that the result of a
   previous check to confirm that certificates in the chain are valid CA
   certificates was overwritten. This effectively bypasses the check
   that non-CA certificates must not be able to issue other certificates.

   If a "purpose" has been configured then there is a subsequent opportunity
   for checks that the certificate is a valid CA.  All of the named "purpose"
   values implemented in libcrypto perform this check.  Therefore, where
   a purpose is set the certificate chain will still be rejected even when the
   strict flag has been used. A purpose is set by default in libssl client and
   server certificate verification routines, but it can be overridden or
   removed by an application.

   In order to be affected, an application must explicitly set the
   X509_V_FLAG_X509_STRICT verification flag and either not set a purpose
   for the certificate verification or, in the case of TLS client or server
   applications, override the default purpose.
   ([CVE-2021-3450])

   *Tomáš Mráz*

 * Fixed an issue where an OpenSSL TLS server may crash if sent a maliciously
   crafted renegotiation ClientHello message from a client. If a TLSv1.2
   renegotiation ClientHello omits the signature_algorithms extension (where it
   was present in the initial ClientHello), but includes a
   signature_algorithms_cert extension then a NULL pointer dereference will
   result, leading to a crash and a denial of service attack.

   A server is only vulnerable if it has TLSv1.2 and renegotiation enabled
   (which is the default configuration). OpenSSL TLS clients are not impacted by
   this issue.
   ([CVE-2021-3449])

   *Peter Kästle and Samuel Sapalski*

### Changes between 1.1.1i and 1.1.1j [16 Feb 2021]

 * Fixed the X509_issuer_and_serial_hash() function. It attempts to
   create a unique hash value based on the issuer and serial number data
   contained within an X509 certificate. However it was failing to correctly
   handle any errors that may occur while parsing the issuer field (which might
   occur if the issuer field is maliciously constructed). This may subsequently
   result in a NULL pointer deref and a crash leading to a potential denial of
   service attack.
   ([CVE-2021-23841])

   *Matt Caswell*

 * Fixed the RSA_padding_check_SSLv23() function and the RSA_SSLV23_PADDING
   padding mode to correctly check for rollback attacks. This is considered a
   bug in OpenSSL 1.1.1 because it does not support SSLv2. In 1.0.2 this is
   CVE-2021-23839.

   *Matt Caswell*

   Fixed the EVP_CipherUpdate, EVP_EncryptUpdate and EVP_DecryptUpdate
   functions. Previously they could overflow the output length argument in some
   cases where the input length is close to the maximum permissable length for
   an integer on the platform. In such cases the return value from the function
   call would be 1 (indicating success), but the output length value would be
   negative. This could cause applications to behave incorrectly or crash.
   ([CVE-2021-23840])

   *Matt Caswell*

 * Fixed SRP_Calc_client_key so that it runs in constant time. The previous
   implementation called BN_mod_exp without setting BN_FLG_CONSTTIME. This
   could be exploited in a side channel attack to recover the password. Since
   the attack is local host only this is outside of the current OpenSSL
   threat model and therefore no CVE is assigned.

   Thanks to Mohammed Sabt and Daniel De Almeida Braga for reporting this
   issue.

   *Matt Caswell*

### Changes between 1.1.1h and 1.1.1i [8 Dec 2020]

 * Fixed NULL pointer deref in the GENERAL_NAME_cmp function
   This function could crash if both GENERAL_NAMEs contain an EDIPARTYNAME.
    If an attacker can control both items being compared  then this could lead
    to a possible denial of service attack. OpenSSL itself uses the
    GENERAL_NAME_cmp function for two purposes:
    1) Comparing CRL distribution point names between an available CRL and a
       CRL distribution point embedded in an X509 certificate
    2) When verifying that a timestamp response token signer matches the
       timestamp authority name (exposed via the API functions
       TS_RESP_verify_response and TS_RESP_verify_token)
   ([CVE-2020-1971])

   *Matt Caswell*

### Changes between 1.1.1g and 1.1.1h [22 Sep 2020]

 * Certificates with explicit curve parameters are now disallowed in
   verification chains if the X509_V_FLAG_X509_STRICT flag is used.

   *Tomáš Mráz*

 * The 'MinProtocol' and 'MaxProtocol' configuration commands now silently
   ignore TLS protocol version bounds when configuring DTLS-based contexts, and
   conversely, silently ignore DTLS protocol version bounds when configuring
   TLS-based contexts.  The commands can be repeated to set bounds of both
   types.  The same applies with the corresponding "min_protocol" and
   "max_protocol" command-line switches, in case some application uses both TLS
   and DTLS.

   SSL_CTX instances that are created for a fixed protocol version (e.g.
   TLSv1_server_method()) also silently ignore version bounds.  Previously
   attempts to apply bounds to these protocol versions would result in an
   error.  Now only the "version-flexible" SSL_CTX instances are subject to
   limits in configuration files in command-line options.

   *Viktor Dukhovni*

 * Handshake now fails if Extended Master Secret extension is dropped
   on renegotiation.

   *Tomáš Mráz*

 * The Oracle Developer Studio compiler will start reporting deprecated APIs

### Changes between 1.1.1f and 1.1.1g [21 Apr 2020]

 * Fixed segmentation fault in SSL_check_chain()
   Server or client applications that call the SSL_check_chain() function
   during or after a TLS 1.3 handshake may crash due to a NULL pointer
   dereference as a result of incorrect handling of the
   "signature_algorithms_cert" TLS extension. The crash occurs if an invalid
   or unrecognised signature algorithm is received from the peer. This could
   be exploited by a malicious peer in a Denial of Service attack.
   ([CVE-2020-1967])

   *Benjamin Kaduk*

 * Added AES consttime code for no-asm configurations
   an optional constant time support for AES was added
   when building openssl for no-asm.
   Enable with: ./config no-asm -DOPENSSL_AES_CONST_TIME
   Disable with: ./config no-asm -DOPENSSL_NO_AES_CONST_TIME
   At this time this feature is by default disabled.
   It will be enabled by default in 3.0.

   *Bernd Edlinger*

### Changes between 1.1.1e and 1.1.1f [31 Mar 2020]

 * Revert the change of EOF detection while reading in libssl to avoid
   regressions in applications depending on the current way of reporting
   the EOF. As the existing method is not fully accurate the change to
   reporting the EOF via SSL_ERROR_SSL is kept on the current development
   branch and will be present in the 3.0 release.

   *Tomáš Mráz*

 * Revised BN_generate_prime_ex to not avoid factors 3..17863 in p-1
   when primes for RSA keys are computed.
   Since we previously always generated primes == 2 (mod 3) for RSA keys,
   the 2-prime and 3-prime RSA modules were easy to distinguish, since
   N = p*q = 1 (mod 3), but N = p*q*r = 2 (mod 3). Therefore fingerprinting
   2-prime vs. 3-prime RSA keys was possible by computing N mod 3.
   This avoids possible fingerprinting of newly generated RSA modules.

   *Bernd Edlinger*

### Changes between 1.1.1d and 1.1.1e [17 Mar 2020]

 * Properly detect EOF while reading in libssl. Previously if we hit an EOF
   while reading in libssl then we would report an error back to the
   application (SSL_ERROR_SYSCALL) but errno would be 0. We now add
   an error to the stack (which means we instead return SSL_ERROR_SSL) and
   therefore give a hint as to what went wrong.

   *Matt Caswell*

 * Check that ed25519 and ed448 are allowed by the security level. Previously
   signature algorithms not using an MD were not being checked that they were
   allowed by the security level.

   *Kurt Roeckx*

 * Fixed SSL_get_servername() behaviour. The behaviour of SSL_get_servername()
   was not quite right. The behaviour was not consistent between resumption
   and normal handshakes, and also not quite consistent with historical
   behaviour. The behaviour in various scenarios has been clarified and
   it has been updated to make it match historical behaviour as closely as
   possible.

   *Matt Caswell*

 * *[VMS only]* The header files that the VMS compilers include automatically,
   `__DECC_INCLUDE_PROLOGUE.H` and `__DECC_INCLUDE_EPILOGUE.H`, use pragmas
   that the C++ compiler doesn't understand.  This is a shortcoming in the
   compiler, but can be worked around with `__cplusplus` guards.

   C++ applications that use OpenSSL libraries must be compiled using the
   qualifier `/NAMES=(AS_IS,SHORTENED)` to be able to use all the OpenSSL
   functions.  Otherwise, only functions with symbols of less than 31
   characters can be used, as the linker will not be able to successfully
   resolve symbols with longer names.

   *Richard Levitte*

 * Added a new method to gather entropy on VMS, based on SYS$GET_ENTROPY.
   The presence of this system service is determined at run-time.

   *Richard Levitte*

 * Print all values for a PKCS#12 attribute with 'openssl pkcs12', not just
   the first value.

   *Jon Spillett*

### Changes between 1.1.1c and 1.1.1d [10 Sep 2019]

 * Fixed a fork protection issue. OpenSSL 1.1.1 introduced a rewritten random
   number generator (RNG). This was intended to include protection in the
   event of a fork() system call in order to ensure that the parent and child
   processes did not share the same RNG state. However this protection was not
   being used in the default case.

   A partial mitigation for this issue is that the output from a high
   precision timer is mixed into the RNG state so the likelihood of a parent
   and child process sharing state is significantly reduced.

   If an application already calls OPENSSL_init_crypto() explicitly using
   OPENSSL_INIT_ATFORK then this problem does not occur at all.
   ([CVE-2019-1549])

   *Matthias St. Pierre*

 * For built-in EC curves, ensure an EC_GROUP built from the curve name is
   used even when parsing explicit parameters, when loading a encoded key
   or calling `EC_GROUP_new_from_ecpkparameters()`/
   `EC_GROUP_new_from_ecparameters()`.
   This prevents bypass of security hardening and performance gains,
   especially for curves with specialized EC_METHODs.
   By default, if a key encoded with explicit parameters is loaded and later
   encoded, the output is still encoded with explicit parameters, even if
   internally a "named" EC_GROUP is used for computation.

   *Nicola Tuveri*

 * Compute ECC cofactors if not provided during EC_GROUP construction. Before
   this change, EC_GROUP_set_generator would accept order and/or cofactor as
   NULL. After this change, only the cofactor parameter can be NULL. It also
   does some minimal sanity checks on the passed order.
   ([CVE-2019-1547])

   *Billy Bob Brumley*

 * Fixed a padding oracle in PKCS7_dataDecode and CMS_decrypt_set1_pkey.
   An attack is simple, if the first CMS_recipientInfo is valid but the
   second CMS_recipientInfo is chosen ciphertext. If the second
   recipientInfo decodes to PKCS #1 v1.5 form plaintext, the correct
   encryption key will be replaced by garbage, and the message cannot be
   decoded, but if the RSA decryption fails, the correct encryption key is
   used and the recipient will not notice the attack.
   As a work around for this potential attack the length of the decrypted
   key must be equal to the cipher default key length, in case the
   certifiate is not given and all recipientInfo are tried out.
   The old behaviour can be re-enabled in the CMS code by setting the
   CMS_DEBUG_DECRYPT flag.
   ([CVE-2019-1563])

   *Bernd Edlinger*

 * Early start up entropy quality from the DEVRANDOM seed source has been
   improved for older Linux systems.  The RAND subsystem will wait for
   /dev/random to be producing output before seeding from /dev/urandom.
   The seeded state is stored for future library initialisations using
   a system global shared memory segment.  The shared memory identifier
   can be configured by defining OPENSSL_RAND_SEED_DEVRANDOM_SHM_ID to
   the desired value.  The default identifier is 114.

   *Paul Dale*

 * Correct the extended master secret constant on EBCDIC systems. Without this
   fix TLS connections between an EBCDIC system and a non-EBCDIC system that
   negotiate EMS will fail. Unfortunately this also means that TLS connections
   between EBCDIC systems with this fix, and EBCDIC systems without this
   fix will fail if they negotiate EMS.

   *Matt Caswell*

 * Use Windows installation paths in the mingw builds

   Mingw isn't a POSIX environment per se, which means that Windows
   paths should be used for installation.
   ([CVE-2019-1552])

   *Richard Levitte*

 * Changed DH_check to accept parameters with order q and 2q subgroups.
   With order 2q subgroups the bit 0 of the private key is not secret
   but DH_generate_key works around that by clearing bit 0 of the
   private key for those. This avoids leaking bit 0 of the private key.

   *Bernd Edlinger*

 * Significantly reduce secure memory usage by the randomness pools.

   *Paul Dale*

 * Revert the DEVRANDOM_WAIT feature for Linux systems

   The DEVRANDOM_WAIT feature added a select() call to wait for the
   /dev/random device to become readable before reading from the
   /dev/urandom device.

   It turned out that this change had negative side effects on
   performance which were not acceptable. After some discussion it
   was decided to revert this feature and leave it up to the OS
   resp. the platform maintainer to ensure a proper initialization
   during early boot time.

   *Matthias St. Pierre*

### Changes between 1.1.1b and 1.1.1c [28 May 2019]

 * Add build tests for C++.  These are generated files that only do one
   thing, to include one public OpenSSL head file each.  This tests that
   the public header files can be usefully included in a C++ application.

   This test isn't enabled by default.  It can be enabled with the option
   'enable-buildtest-c++'.

   *Richard Levitte*

 * Enable SHA3 pre-hashing for ECDSA and DSA.

   *Patrick Steuer*

 * Change the default RSA, DSA and DH size to 2048 bit instead of 1024.
   This changes the size when using the `genpkey` command when no size is given.
   It fixes an omission in earlier changes that changed all RSA, DSA and DH
   generation commands to use 2048 bits by default.

   *Kurt Roeckx*

 * Reorganize the manual pages to consistently have RETURN VALUES,
   EXAMPLES, SEE ALSO and HISTORY come in that order, and adjust
   util/fix-doc-nits accordingly.

   *Paul Yang, Joshua Lock*

 * Add the missing accessor EVP_PKEY_get0_engine()

   *Matt Caswell*

 * Have commands like `s_client` and `s_server` output the signature scheme
   along with other cipher suite parameters when debugging.

   *Lorinczy Zsigmond*

 * Make OPENSSL_config() error agnostic again.

   *Richard Levitte*

 * Do the error handling in RSA decryption constant time.

   *Bernd Edlinger*

 * Prevent over long nonces in ChaCha20-Poly1305.

   ChaCha20-Poly1305 is an AEAD cipher, and requires a unique nonce input
   for every encryption operation. RFC 7539 specifies that the nonce value
   (IV) should be 96 bits (12 bytes). OpenSSL allows a variable nonce length
   and front pads the nonce with 0 bytes if it is less than 12
   bytes. However it also incorrectly allows a nonce to be set of up to 16
   bytes. In this case only the last 12 bytes are significant and any
   additional leading bytes are ignored.

   It is a requirement of using this cipher that nonce values are
   unique. Messages encrypted using a reused nonce value are susceptible to
   serious confidentiality and integrity attacks. If an application changes
   the default nonce length to be longer than 12 bytes and then makes a
   change to the leading bytes of the nonce expecting the new value to be a
   new unique nonce then such an application could inadvertently encrypt
   messages with a reused nonce.

   Additionally the ignored bytes in a long nonce are not covered by the
   integrity guarantee of this cipher. Any application that relies on the
   integrity of these ignored leading bytes of a long nonce may be further
   affected. Any OpenSSL internal use of this cipher, including in SSL/TLS,
   is safe because no such use sets such a long nonce value. However user
   applications that use this cipher directly and set a non-default nonce
   length to be longer than 12 bytes may be vulnerable.

   This issue was reported to OpenSSL on 16th of March 2019 by Joran Dirk
   Greef of Ronomon.
   ([CVE-2019-1543])

   *Matt Caswell*

 * Add DEVRANDOM_WAIT feature for Linux systems

   On older Linux systems where the getrandom() system call is not available,
   OpenSSL normally uses the /dev/urandom device for seeding its CSPRNG.
   Contrary to getrandom(), the /dev/urandom device will not block during
   early boot when the kernel CSPRNG has not been seeded yet.

   To mitigate this known weakness, use select() to wait for /dev/random to
   become readable before reading from /dev/urandom.

 * Ensure that SM2 only uses SM3 as digest algorithm

   *Paul Yang*

### Changes between 1.1.1a and 1.1.1b [26 Feb 2019]

 * Change the info callback signals for the start and end of a post-handshake
   message exchange in TLSv1.3. In 1.1.1/1.1.1a we used SSL_CB_HANDSHAKE_START
   and SSL_CB_HANDSHAKE_DONE. Experience has shown that many applications get
   confused by this and assume that a TLSv1.2 renegotiation has started. This
   can break KeyUpdate handling. Instead we no longer signal the start and end
   of a post handshake message exchange (although the messages themselves are
   still signalled). This could break some applications that were expecting
   the old signals. However without this KeyUpdate is not usable for many
   applications.

   *Matt Caswell*

### Changes between 1.1.1 and 1.1.1a [20 Nov 2018]

 * Timing vulnerability in DSA signature generation

   The OpenSSL DSA signature algorithm has been shown to be vulnerable to a
   timing side channel attack. An attacker could use variations in the signing
   algorithm to recover the private key.

   This issue was reported to OpenSSL on 16th October 2018 by Samuel Weiser.
   ([CVE-2018-0734])

   *Paul Dale*

 * Timing vulnerability in ECDSA signature generation

   The OpenSSL ECDSA signature algorithm has been shown to be vulnerable to a
   timing side channel attack. An attacker could use variations in the signing
   algorithm to recover the private key.

   This issue was reported to OpenSSL on 25th October 2018 by Samuel Weiser.
   ([CVE-2018-0735])

   *Paul Dale*

 * Fixed the issue that RAND_add()/RAND_seed() silently discards random input
   if its length exceeds 4096 bytes. The limit has been raised to a buffer size
   of two gigabytes and the error handling improved.

   This issue was reported to OpenSSL by Dr. Falko Strenzke. It has been
   categorized as a normal bug, not a security issue, because the DRBG reseeds
   automatically and is fully functional even without additional randomness
   provided by the application.

### Changes between 1.1.0i and 1.1.1 [11 Sep 2018]

 * Add a new ClientHello callback. Provides a callback interface that gives
   the application the ability to adjust the nascent SSL object at the
   earliest stage of ClientHello processing, immediately after extensions have
   been collected but before they have been processed. In particular, this
   callback can adjust the supported TLS versions in response to the contents
   of the ClientHello

   *Benjamin Kaduk*

 * Add SM2 base algorithm support.

   *Jack Lloyd*

 * s390x assembly pack: add (improved) hardware-support for the following
   cryptographic primitives: sha3, shake, aes-gcm, aes-ccm, aes-ctr, aes-ofb,
   aes-cfb/cfb8, aes-ecb.

   *Patrick Steuer*

 * Make EVP_PKEY_asn1_new() a bit stricter about its input.  A NULL pem_str
   parameter is no longer accepted, as it leads to a corrupt table.  NULL
   pem_str is reserved for alias entries only.

   *Richard Levitte*

 * Use the new ec_scalar_mul_ladder scaffold to implement a specialized ladder
   step for prime curves. The new implementation is based on formulae from
   differential addition-and-doubling in homogeneous projective coordinates
   from Izu-Takagi "A fast parallel elliptic curve multiplication resistant
   against side channel attacks" and Brier-Joye "Weierstrass Elliptic Curves
   and Side-Channel Attacks" Eq. (8) for y-coordinate recovery, modified
   to work in projective coordinates.

   *Billy Bob Brumley, Nicola Tuveri*

 * Change generating and checking of primes so that the error rate of not
   being prime depends on the intended use based on the size of the input.
   For larger primes this will result in more rounds of Miller-Rabin.
   The maximal error rate for primes with more than 1080 bits is lowered
   to 2^-128.

   *Kurt Roeckx, Annie Yousar*

 * Increase the number of Miller-Rabin rounds for DSA key generating to 64.

   *Kurt Roeckx*

 * The 'tsget' script is renamed to 'tsget.pl', to avoid confusion when
   moving between systems, and to avoid confusion when a Windows build is
   done with mingw vs with MSVC.  For POSIX installs, there's still a
   symlink or copy named 'tsget' to avoid that confusion as well.

   *Richard Levitte*

 * Revert blinding in ECDSA sign and instead make problematic addition
   length-invariant. Switch even to fixed-length Montgomery multiplication.

   *Andy Polyakov*

 * Use the new ec_scalar_mul_ladder scaffold to implement a specialized ladder
   step for binary curves. The new implementation is based on formulae from
   differential addition-and-doubling in mixed Lopez-Dahab projective
   coordinates, modified to independently blind the operands.

   *Billy Bob Brumley, Sohaib ul Hassan, Nicola Tuveri*

 * Add a scaffold to optionally enhance the Montgomery ladder implementation
   for `ec_scalar_mul_ladder` (formerly `ec_mul_consttime`) allowing
   EC_METHODs to implement their own specialized "ladder step", to take
   advantage of more favorable coordinate systems or more efficient
   differential addition-and-doubling algorithms.

   *Billy Bob Brumley, Sohaib ul Hassan, Nicola Tuveri*

 * Modified the random device based seed sources to keep the relevant
   file descriptors open rather than reopening them on each access.
   This allows such sources to operate in a chroot() jail without
   the associated device nodes being available. This behaviour can be
   controlled using RAND_keep_random_devices_open().

   *Paul Dale*

 * Numerous side-channel attack mitigations have been applied. This may have
   performance impacts for some algorithms for the benefit of improved
   security. Specific changes are noted in this change log by their respective
   authors.

   *Matt Caswell*

 * AIX shared library support overhaul. Switch to AIX "natural" way of
   handling shared libraries, which means collecting shared objects of
   different versions and bitnesses in one common archive. This allows to
   mitigate conflict between 1.0 and 1.1 side-by-side installations. It
   doesn't affect the way 3rd party applications are linked, only how
   multi-version installation is managed.

   *Andy Polyakov*

 * Make ec_group_do_inverse_ord() more robust and available to other
   EC cryptosystems, so that irrespective of BN_FLG_CONSTTIME, SCA
   mitigations are applied to the fallback BN_mod_inverse().
   When using this function rather than BN_mod_inverse() directly, new
   EC cryptosystem implementations are then safer-by-default.

   *Billy Bob Brumley*

 * Add coordinate blinding for EC_POINT and implement projective
   coordinate blinding for generic prime curves as a countermeasure to
   chosen point SCA attacks.

   *Sohaib ul Hassan, Nicola Tuveri, Billy Bob Brumley*

 * Add blinding to ECDSA and DSA signatures to protect against side channel
   attacks discovered by Keegan Ryan (NCC Group).

   *Matt Caswell*

 * Enforce checking in the `pkeyutl` command to ensure that the input
   length does not exceed the maximum supported digest length when performing
   a sign, verify or verifyrecover operation.

   *Matt Caswell*

 * SSL_MODE_AUTO_RETRY is enabled by default. Applications that use blocking
   I/O in combination with something like select() or poll() will hang. This
   can be turned off again using SSL_CTX_clear_mode().
   Many applications do not properly handle non-application data records, and
   TLS 1.3 sends more of such records. Setting SSL_MODE_AUTO_RETRY works
   around the problems in those applications, but can also break some.
   It's recommended to read the manpages about SSL_read(), SSL_write(),
   SSL_get_error(), SSL_shutdown(), SSL_CTX_set_mode() and
   SSL_CTX_set_read_ahead() again.

   *Kurt Roeckx*

 * When unlocking a pass phrase protected PEM file or PKCS#8 container, we
   now allow empty (zero character) pass phrases.

   *Richard Levitte*

 * Apply blinding to binary field modular inversion and remove patent
   pending (OPENSSL_SUN_GF2M_DIV) BN_GF2m_mod_div implementation.

   *Billy Bob Brumley*

 * Deprecate ec2_mult.c and unify scalar multiplication code paths for
   binary and prime elliptic curves.

   *Billy Bob Brumley*

 * Remove ECDSA nonce padding: EC_POINT_mul is now responsible for
   constant time fixed point multiplication.

   *Billy Bob Brumley*

 * Revise elliptic curve scalar multiplication with timing attack
   defenses: ec_wNAF_mul redirects to a constant time implementation
   when computing fixed point and variable point multiplication (which
   in OpenSSL are mostly used with secret scalars in keygen, sign,
   ECDH derive operations).
   *Billy Bob Brumley, Nicola Tuveri, Cesar Pereida García,
    Sohaib ul Hassan*

 * Updated CONTRIBUTING

   *Rich Salz*

 * Updated DRBG / RAND to request nonce and additional low entropy
   randomness from the system.

   *Matthias St. Pierre*

 * Updated 'openssl rehash' to use OpenSSL consistent default.

   *Richard Levitte*

 * Moved the load of the ssl_conf module to libcrypto, which helps
   loading engines that libssl uses before libssl is initialised.

   *Matt Caswell*

 * Added EVP_PKEY_sign() and EVP_PKEY_verify() for EdDSA

   *Matt Caswell*

 * Fixed X509_NAME_ENTRY_set to get multi-valued RDNs right in all cases.

   *Ingo Schwarze, Rich Salz*

 * Added output of accepting IP address and port for 'openssl s_server'

   *Richard Levitte*

 * Added a new API for TLSv1.3 ciphersuites:
      SSL_CTX_set_ciphersuites()
      SSL_set_ciphersuites()

   *Matt Caswell*

 * Memory allocation failures consistently add an error to the error
   stack.

   *Rich Salz*

 * Don't use OPENSSL_ENGINES and OPENSSL_CONF environment values
   in libcrypto when run as setuid/setgid.

   *Bernd Edlinger*

 * Load any config file by default when libssl is used.

   *Matt Caswell*

 * Added new public header file <openssl/rand_drbg.h> and documentation
   for the RAND_DRBG API. See manual page RAND_DRBG(7) for an overview.

   *Matthias St. Pierre*

 * QNX support removed (cannot find contributors to get their approval
   for the license change).

   *Rich Salz*

 * TLSv1.3 replay protection for early data has been implemented. See the
   SSL_read_early_data() man page for further details.

   *Matt Caswell*

 * Separated TLSv1.3 ciphersuite configuration out from TLSv1.2 ciphersuite
   configuration. TLSv1.3 ciphersuites are not compatible with TLSv1.2 and
   below. Similarly TLSv1.2 ciphersuites are not compatible with TLSv1.3.
   In order to avoid issues where legacy TLSv1.2 ciphersuite configuration
   would otherwise inadvertently disable all TLSv1.3 ciphersuites the
   configuration has been separated out. See the ciphers man page or the
   SSL_CTX_set_ciphersuites() man page for more information.

   *Matt Caswell*

 * On POSIX (BSD, Linux, ...) systems the ocsp(1) command running
   in responder mode now supports the new "-multi" option, which
   spawns the specified number of child processes to handle OCSP
   requests.  The "-timeout" option now also limits the OCSP
   responder's patience to wait to receive the full client request
   on a newly accepted connection. Child processes are respawned
   as needed, and the CA index file is automatically reloaded
   when changed.  This makes it possible to run the "ocsp" responder
   as a long-running service, making the OpenSSL CA somewhat more
   feature-complete.  In this mode, most diagnostic messages logged
   after entering the event loop are logged via syslog(3) rather than
   written to stderr.

   *Viktor Dukhovni*

 * Added support for X448 and Ed448. Heavily based on original work by
   Mike Hamburg.

   *Matt Caswell*

 * Extend OSSL_STORE with capabilities to search and to narrow the set of
   objects loaded.  This adds the functions OSSL_STORE_expect() and
   OSSL_STORE_find() as well as needed tools to construct searches and
   get the search data out of them.

   *Richard Levitte*

 * Support for TLSv1.3 added. Note that users upgrading from an earlier
   version of OpenSSL should review their configuration settings to ensure
   that they are still appropriate for TLSv1.3. For further information see:
   <https://wiki.openssl.org/index.php/TLS1.3>

   *Matt Caswell*

 * Grand redesign of the OpenSSL random generator

   The default RAND method now utilizes an AES-CTR DRBG according to
   NIST standard SP 800-90Ar1. The new random generator is essentially
   a port of the default random generator from the OpenSSL FIPS 2.0
   object module. It is a hybrid deterministic random bit generator
   using an AES-CTR bit stream and which seeds and reseeds itself
   automatically using trusted system entropy sources.

   Some of its new features are:
    - Support for multiple DRBG instances with seed chaining.
    - The default RAND method makes use of a DRBG.
    - There is a public and private DRBG instance.
    - The DRBG instances are fork-safe.
    - Keep all global DRBG instances on the secure heap if it is enabled.
    - The public and private DRBG instance are per thread for lock free
      operation

   *Paul Dale, Benjamin Kaduk, Kurt Roeckx, Rich Salz, Matthias St. Pierre*

 * Changed Configure so it only says what it does and doesn't dump
   so much data.  Instead, ./configdata.pm should be used as a script
   to display all sorts of configuration data.

   *Richard Levitte*

 * Added processing of "make variables" to Configure.

   *Richard Levitte*

 * Added SHA512/224 and SHA512/256 algorithm support.

   *Paul Dale*

 * The last traces of Netware support, first removed in 1.1.0, have
   now been removed.

   *Rich Salz*

 * Get rid of Makefile.shared, and in the process, make the processing
   of certain files (rc.obj, or the .def/.map/.opt files produced from
   the ordinal files) more visible and hopefully easier to trace and
   debug (or make silent).

   *Richard Levitte*

 * Make it possible to have environment variable assignments as
   arguments to config / Configure.

   *Richard Levitte*

 * Add multi-prime RSA (RFC 8017) support.

   *Paul Yang*

 * Add SM3 implemented according to GB/T 32905-2016
   *Jack Lloyd <jack.lloyd@ribose.com>,*
   *Ronald Tse <ronald.tse@ribose.com>,*
   *Erick Borsboom <erick.borsboom@ribose.com>*

 * Add 'Maximum Fragment Length' TLS extension negotiation and support
   as documented in RFC6066.
   Based on a patch from Tomasz Moń

   *Filipe Raimundo da Silva*

 * Add SM4 implemented according to GB/T 32907-2016.
   *Jack Lloyd <jack.lloyd@ribose.com>,*
   *Ronald Tse <ronald.tse@ribose.com>,*
   *Erick Borsboom <erick.borsboom@ribose.com>*

 * Reimplement -newreq-nodes and ERR_error_string_n; the
   original author does not agree with the license change.

   *Rich Salz*

 * Add ARIA AEAD TLS support.

   *Jon Spillett*

 * Some macro definitions to support VS6 have been removed.  Visual
   Studio 6 has not worked since 1.1.0

   *Rich Salz*

 * Add ERR_clear_last_mark(), to allow callers to clear the last mark
   without clearing the errors.

   *Richard Levitte*

 * Add "atfork" functions.  If building on a system that without
   pthreads, see doc/man3/OPENSSL_fork_prepare.pod for application
   requirements.  The RAND facility now uses/requires this.

   *Rich Salz*

 * Add SHA3.

   *Andy Polyakov*

 * The UI API becomes a permanent and integral part of libcrypto, i.e.
   not possible to disable entirely.  However, it's still possible to
   disable the console reading UI method, UI_OpenSSL() (use UI_null()
   as a fallback).

   To disable, configure with 'no-ui-console'.  'no-ui' is still
   possible to use as an alias.  Check at compile time with the
   macro OPENSSL_NO_UI_CONSOLE.  The macro OPENSSL_NO_UI is still
   possible to check and is an alias for OPENSSL_NO_UI_CONSOLE.

   *Richard Levitte*

 * Add a STORE module, which implements a uniform and URI based reader of
   stores that can contain keys, certificates, CRLs and numerous other
   objects.  The main API is loosely based on a few stdio functions,
   and includes OSSL_STORE_open, OSSL_STORE_load, OSSL_STORE_eof,
   OSSL_STORE_error and OSSL_STORE_close.
   The implementation uses backends called "loaders" to implement arbitrary
   URI schemes.  There is one built in "loader" for the 'file' scheme.

   *Richard Levitte*

 * Add devcrypto engine.  This has been implemented against cryptodev-linux,
   then adjusted to work on FreeBSD 8.4 as well.
   Enable by configuring with 'enable-devcryptoeng'.  This is done by default
   on BSD implementations, as cryptodev.h is assumed to exist on all of them.

   *Richard Levitte*

 * Module names can prefixed with OSSL_ or OPENSSL_.  This affects
   util/mkerr.pl, which is adapted to allow those prefixes, leading to
   error code calls like this:

           OSSL_FOOerr(OSSL_FOO_F_SOMETHING, OSSL_FOO_R_WHATEVER);

   With this change, we claim the namespaces OSSL and OPENSSL in a manner
   that can be encoded in C.  For the foreseeable future, this will only
   affect new modules.

   *Richard Levitte and Tim Hudson*

 * Removed BSD cryptodev engine.

   *Rich Salz*

 * Add a build target 'build_all_generated', to build all generated files
   and only that.  This can be used to prepare everything that requires
   things like perl for a system that lacks perl and then move everything
   to that system and do the rest of the build there.

   *Richard Levitte*

 * In the UI interface, make it possible to duplicate the user data.  This
   can be used by engines that need to retain the data for a longer time
   than just the call where this user data is passed.

   *Richard Levitte*

 * Ignore the '-named_curve auto' value for compatibility of applications
   with OpenSSL 1.0.2.

   *Tomáš Mráz <tmraz@fedoraproject.org>*

 * Fragmented SSL/TLS alerts are no longer accepted. An alert message is 2
   bytes long. In theory it is permissible in SSLv3 - TLSv1.2 to fragment such
   alerts across multiple records (some of which could be empty). In practice
   it make no sense to send an empty alert record, or to fragment one. TLSv1.3
   prohibits this altogether and other libraries (BoringSSL, NSS) do not
   support this at all. Supporting it adds significant complexity to the
   record layer, and its removal is unlikely to cause interoperability
   issues.

   *Matt Caswell*

 * Add the ASN.1 types INT32, UINT32, INT64, UINT64 and variants prefixed
   with Z.  These are meant to replace LONG and ZLONG and to be size safe.
   The use of LONG and ZLONG is discouraged and scheduled for deprecation
   in OpenSSL 1.2.0.

   *Richard Levitte*

 * Add the 'z' and 'j' modifiers to BIO_printf() et al formatting string,
   'z' is to be used for [s]size_t, and 'j' - with [u]int64_t.

   *Richard Levitte, Andy Polyakov*

 * Add EC_KEY_get0_engine(), which does for EC_KEY what RSA_get0_engine()
   does for RSA, etc.

   *Richard Levitte*

 * Have 'config' recognise 64-bit mingw and choose 'mingw64' as the target
   platform rather than 'mingw'.

   *Richard Levitte*

 * The functions X509_STORE_add_cert and X509_STORE_add_crl return
   success if they are asked to add an object which already exists
   in the store. This change cascades to other functions which load
   certificates and CRLs.

   *Paul Dale*

 * x86_64 assembly pack: annotate code with DWARF CFI directives to
   facilitate stack unwinding even from assembly subroutines.

   *Andy Polyakov*

 * Remove VAX C specific definitions of OPENSSL_EXPORT, OPENSSL_EXTERN.
   Also remove OPENSSL_GLOBAL entirely, as it became a no-op.

   *Richard Levitte*

 * Remove the VMS-specific reimplementation of gmtime from crypto/o_times.c.
   VMS C's RTL has a fully up to date gmtime() and gmtime_r() since V7.1,
   which is the minimum version we support.

   *Richard Levitte*

 * Certificate time validation (X509_cmp_time) enforces stricter
   compliance with RFC 5280. Fractional seconds and timezone offsets
   are no longer allowed.

   *Emilia Käsper*

 * Add support for ARIA

   *Paul Dale*

 * s_client will now send the Server Name Indication (SNI) extension by
   default unless the new "-noservername" option is used. The server name is
   based on the host provided to the "-connect" option unless overridden by
   using "-servername".

   *Matt Caswell*

 * Add support for SipHash

   *Todd Short*

 * OpenSSL now fails if it receives an unrecognised record type in TLS1.0
   or TLS1.1. Previously this only happened in SSLv3 and TLS1.2. This is to
   prevent issues where no progress is being made and the peer continually
   sends unrecognised record types, using up resources processing them.

   *Matt Caswell*

 * 'openssl passwd' can now produce SHA256 and SHA512 based output,
   using the algorithm defined in
   <https://www.akkadia.org/drepper/SHA-crypt.txt>

   *Richard Levitte*

 * Heartbeat support has been removed; the ABI is changed for now.

   *Richard Levitte, Rich Salz*

 * Support for SSL_OP_NO_ENCRYPT_THEN_MAC in SSL_CONF_cmd.

   *Emilia Käsper*

 * The RSA "null" method, which was partially supported to avoid patent
   issues, has been replaced to always returns NULL.

   *Rich Salz*

OpenSSL 1.1.0
-------------

### Changes between 1.1.0k and 1.1.0l [10 Sep 2019]

 * For built-in EC curves, ensure an EC_GROUP built from the curve name is
   used even when parsing explicit parameters, when loading a encoded key
   or calling `EC_GROUP_new_from_ecpkparameters()`/
   `EC_GROUP_new_from_ecparameters()`.
   This prevents bypass of security hardening and performance gains,
   especially for curves with specialized EC_METHODs.
   By default, if a key encoded with explicit parameters is loaded and later
   encoded, the output is still encoded with explicit parameters, even if
   internally a "named" EC_GROUP is used for computation.

   *Nicola Tuveri*

 * Compute ECC cofactors if not provided during EC_GROUP construction. Before
   this change, EC_GROUP_set_generator would accept order and/or cofactor as
   NULL. After this change, only the cofactor parameter can be NULL. It also
   does some minimal sanity checks on the passed order.
   ([CVE-2019-1547])

   *Billy Bob Brumley*

 * Fixed a padding oracle in PKCS7_dataDecode and CMS_decrypt_set1_pkey.
   An attack is simple, if the first CMS_recipientInfo is valid but the
   second CMS_recipientInfo is chosen ciphertext. If the second
   recipientInfo decodes to PKCS #1 v1.5 form plaintext, the correct
   encryption key will be replaced by garbage, and the message cannot be
   decoded, but if the RSA decryption fails, the correct encryption key is
   used and the recipient will not notice the attack.
   As a work around for this potential attack the length of the decrypted
   key must be equal to the cipher default key length, in case the
   certifiate is not given and all recipientInfo are tried out.
   The old behaviour can be re-enabled in the CMS code by setting the
   CMS_DEBUG_DECRYPT flag.
   ([CVE-2019-1563])

   *Bernd Edlinger*

 * Use Windows installation paths in the mingw builds

   Mingw isn't a POSIX environment per se, which means that Windows
   paths should be used for installation.
   ([CVE-2019-1552])

   *Richard Levitte*

### Changes between 1.1.0j and 1.1.0k [28 May 2019]

 * Change the default RSA, DSA and DH size to 2048 bit instead of 1024.
   This changes the size when using the `genpkey` command when no size is given.
   It fixes an omission in earlier changes that changed all RSA, DSA and DH
   generation commands to use 2048 bits by default.

   *Kurt Roeckx*

 * Prevent over long nonces in ChaCha20-Poly1305.

   ChaCha20-Poly1305 is an AEAD cipher, and requires a unique nonce input
   for every encryption operation. RFC 7539 specifies that the nonce value
   (IV) should be 96 bits (12 bytes). OpenSSL allows a variable nonce length
   and front pads the nonce with 0 bytes if it is less than 12
   bytes. However it also incorrectly allows a nonce to be set of up to 16
   bytes. In this case only the last 12 bytes are significant and any
   additional leading bytes are ignored.

   It is a requirement of using this cipher that nonce values are
   unique. Messages encrypted using a reused nonce value are susceptible to
   serious confidentiality and integrity attacks. If an application changes
   the default nonce length to be longer than 12 bytes and then makes a
   change to the leading bytes of the nonce expecting the new value to be a
   new unique nonce then such an application could inadvertently encrypt
   messages with a reused nonce.

   Additionally the ignored bytes in a long nonce are not covered by the
   integrity guarantee of this cipher. Any application that relies on the
   integrity of these ignored leading bytes of a long nonce may be further
   affected. Any OpenSSL internal use of this cipher, including in SSL/TLS,
   is safe because no such use sets such a long nonce value. However user
   applications that use this cipher directly and set a non-default nonce
   length to be longer than 12 bytes may be vulnerable.

   This issue was reported to OpenSSL on 16th of March 2019 by Joran Dirk
   Greef of Ronomon.
   ([CVE-2019-1543])

   *Matt Caswell*

 * Added SCA hardening for modular field inversion in EC_GROUP through
   a new dedicated field_inv() pointer in EC_METHOD.
   This also addresses a leakage affecting conversions from projective
   to affine coordinates.

   *Billy Bob Brumley, Nicola Tuveri*

 * Fix a use after free bug in d2i_X509_PUBKEY when overwriting a
   re-used X509_PUBKEY object if the second PUBKEY is malformed.

   *Bernd Edlinger*

 * Move strictness check from EVP_PKEY_asn1_new() to EVP_PKEY_asn1_add0().

   *Richard Levitte*

 * Remove the 'dist' target and add a tarball building script.  The
   'dist' target has fallen out of use, and it shouldn't be
   necessary to configure just to create a source distribution.

   *Richard Levitte*

### Changes between 1.1.0i and 1.1.0j [20 Nov 2018]

 * Timing vulnerability in DSA signature generation

   The OpenSSL DSA signature algorithm has been shown to be vulnerable to a
   timing side channel attack. An attacker could use variations in the signing
   algorithm to recover the private key.

   This issue was reported to OpenSSL on 16th October 2018 by Samuel Weiser.
   ([CVE-2018-0734])

   *Paul Dale*

 * Timing vulnerability in ECDSA signature generation

   The OpenSSL ECDSA signature algorithm has been shown to be vulnerable to a
   timing side channel attack. An attacker could use variations in the signing
   algorithm to recover the private key.

   This issue was reported to OpenSSL on 25th October 2018 by Samuel Weiser.
   ([CVE-2018-0735])

   *Paul Dale*

 * Add coordinate blinding for EC_POINT and implement projective
   coordinate blinding for generic prime curves as a countermeasure to
   chosen point SCA attacks.

   *Sohaib ul Hassan, Nicola Tuveri, Billy Bob Brumley*

### Changes between 1.1.0h and 1.1.0i [14 Aug 2018]

 * Client DoS due to large DH parameter

   During key agreement in a TLS handshake using a DH(E) based ciphersuite a
   malicious server can send a very large prime value to the client. This will
   cause the client to spend an unreasonably long period of time generating a
   key for this prime resulting in a hang until the client has finished. This
   could be exploited in a Denial Of Service attack.

   This issue was reported to OpenSSL on 5th June 2018 by Guido Vranken
   ([CVE-2018-0732])

   *Guido Vranken*

 * Cache timing vulnerability in RSA Key Generation

   The OpenSSL RSA Key generation algorithm has been shown to be vulnerable to
   a cache timing side channel attack. An attacker with sufficient access to
   mount cache timing attacks during the RSA key generation process could
   recover the private key.

   This issue was reported to OpenSSL on 4th April 2018 by Alejandro Cabrera
   Aldaya, Billy Brumley, Cesar Pereida Garcia and Luis Manuel Alvarez Tapia.
   ([CVE-2018-0737])

   *Billy Brumley*

 * Make EVP_PKEY_asn1_new() a bit stricter about its input.  A NULL pem_str
   parameter is no longer accepted, as it leads to a corrupt table.  NULL
   pem_str is reserved for alias entries only.

   *Richard Levitte*

 * Revert blinding in ECDSA sign and instead make problematic addition
   length-invariant. Switch even to fixed-length Montgomery multiplication.

   *Andy Polyakov*

 * Change generating and checking of primes so that the error rate of not
   being prime depends on the intended use based on the size of the input.
   For larger primes this will result in more rounds of Miller-Rabin.
   The maximal error rate for primes with more than 1080 bits is lowered
   to 2^-128.

   *Kurt Roeckx, Annie Yousar*

 * Increase the number of Miller-Rabin rounds for DSA key generating to 64.

   *Kurt Roeckx*

 * Add blinding to ECDSA and DSA signatures to protect against side channel
   attacks discovered by Keegan Ryan (NCC Group).

   *Matt Caswell*

 * When unlocking a pass phrase protected PEM file or PKCS#8 container, we
   now allow empty (zero character) pass phrases.

   *Richard Levitte*

 * Certificate time validation (X509_cmp_time) enforces stricter
   compliance with RFC 5280. Fractional seconds and timezone offsets
   are no longer allowed.

   *Emilia Käsper*

 * Fixed a text canonicalisation bug in CMS

   Where a CMS detached signature is used with text content the text goes
   through a canonicalisation process first prior to signing or verifying a
   signature. This process strips trailing space at the end of lines, converts
   line terminators to CRLF and removes additional trailing line terminators
   at the end of a file. A bug in the canonicalisation process meant that
   some characters, such as form-feed, were incorrectly treated as whitespace
   and removed. This is contrary to the specification (RFC5485). This fix
   could mean that detached text data signed with an earlier version of
   OpenSSL 1.1.0 may fail to verify using the fixed version, or text data
   signed with a fixed OpenSSL may fail to verify with an earlier version of
   OpenSSL 1.1.0. A workaround is to only verify the canonicalised text data
   and use the "-binary" flag (for the "cms" command line application) or set
   the SMIME_BINARY/PKCS7_BINARY/CMS_BINARY flags (if using CMS_verify()).

   *Matt Caswell*

### Changes between 1.1.0g and 1.1.0h [27 Mar 2018]

 * Constructed ASN.1 types with a recursive definition could exceed the stack

   Constructed ASN.1 types with a recursive definition (such as can be found
   in PKCS7) could eventually exceed the stack given malicious input with
   excessive recursion. This could result in a Denial Of Service attack. There
   are no such structures used within SSL/TLS that come from untrusted sources
   so this is considered safe.

   This issue was reported to OpenSSL on 4th January 2018 by the OSS-fuzz
   project.
   ([CVE-2018-0739])

   *Matt Caswell*

 * Incorrect CRYPTO_memcmp on HP-UX PA-RISC

   Because of an implementation bug the PA-RISC CRYPTO_memcmp function is
   effectively reduced to only comparing the least significant bit of each
   byte. This allows an attacker to forge messages that would be considered as
   authenticated in an amount of tries lower than that guaranteed by the
   security claims of the scheme. The module can only be compiled by the
   HP-UX assembler, so that only HP-UX PA-RISC targets are affected.

   This issue was reported to OpenSSL on 2nd March 2018 by Peter Waltenberg
   (IBM).
   ([CVE-2018-0733])

   *Andy Polyakov*

 * Add a build target 'build_all_generated', to build all generated files
   and only that.  This can be used to prepare everything that requires
   things like perl for a system that lacks perl and then move everything
   to that system and do the rest of the build there.

   *Richard Levitte*

 * Backport SSL_OP_NO_RENGOTIATION

   OpenSSL 1.0.2 and below had the ability to disable renegotiation using the
   (undocumented) SSL3_FLAGS_NO_RENEGOTIATE_CIPHERS flag. Due to the opacity
   changes this is no longer possible in 1.1.0. Therefore the new
   SSL_OP_NO_RENEGOTIATION option from 1.1.1-dev has been backported to
   1.1.0 to provide equivalent functionality.

   Note that if an application built against 1.1.0h headers (or above) is run
   using an older version of 1.1.0 (prior to 1.1.0h) then the option will be
   accepted but nothing will happen, i.e. renegotiation will not be prevented.

   *Matt Caswell*

 * Removed the OS390-Unix config target.  It relied on a script that doesn't
   exist.

   *Rich Salz*

 * rsaz_1024_mul_avx2 overflow bug on x86_64

   There is an overflow bug in the AVX2 Montgomery multiplication procedure
   used in exponentiation with 1024-bit moduli. No EC algorithms are affected.
   Analysis suggests that attacks against RSA and DSA as a result of this
   defect would be very difficult to perform and are not believed likely.
   Attacks against DH1024 are considered just feasible, because most of the
   work necessary to deduce information about a private key may be performed
   offline. The amount of resources required for such an attack would be
   significant. However, for an attack on TLS to be meaningful, the server
   would have to share the DH1024 private key among multiple clients, which is
   no longer an option since CVE-2016-0701.

   This only affects processors that support the AVX2 but not ADX extensions
   like Intel Haswell (4th generation).

   This issue was reported to OpenSSL by David Benjamin (Google). The issue
   was originally found via the OSS-Fuzz project.
   ([CVE-2017-3738])

   *Andy Polyakov*

### Changes between 1.1.0f and 1.1.0g [2 Nov 2017]

 * bn_sqrx8x_internal carry bug on x86_64

   There is a carry propagating bug in the x86_64 Montgomery squaring
   procedure. No EC algorithms are affected. Analysis suggests that attacks
   against RSA and DSA as a result of this defect would be very difficult to
   perform and are not believed likely. Attacks against DH are considered just
   feasible (although very difficult) because most of the work necessary to
   deduce information about a private key may be performed offline. The amount
   of resources required for such an attack would be very significant and
   likely only accessible to a limited number of attackers. An attacker would
   additionally need online access to an unpatched system using the target
   private key in a scenario with persistent DH parameters and a private
   key that is shared between multiple clients.

   This only affects processors that support the BMI1, BMI2 and ADX extensions
   like Intel Broadwell (5th generation) and later or AMD Ryzen.

   This issue was reported to OpenSSL by the OSS-Fuzz project.
   ([CVE-2017-3736])

   *Andy Polyakov*

 * Malformed X.509 IPAddressFamily could cause OOB read

   If an X.509 certificate has a malformed IPAddressFamily extension,
   OpenSSL could do a one-byte buffer overread. The most likely result
   would be an erroneous display of the certificate in text format.

   This issue was reported to OpenSSL by the OSS-Fuzz project.
   ([CVE-2017-3735])

   *Rich Salz*

### Changes between 1.1.0e and 1.1.0f [25 May 2017]

 * Have 'config' recognise 64-bit mingw and choose 'mingw64' as the target
   platform rather than 'mingw'.

   *Richard Levitte*

 * Remove the VMS-specific reimplementation of gmtime from crypto/o_times.c.
   VMS C's RTL has a fully up to date gmtime() and gmtime_r() since V7.1,
   which is the minimum version we support.

   *Richard Levitte*

### Changes between 1.1.0d and 1.1.0e [16 Feb 2017]

 * Encrypt-Then-Mac renegotiation crash

   During a renegotiation handshake if the Encrypt-Then-Mac extension is
   negotiated where it was not in the original handshake (or vice-versa) then
   this can cause OpenSSL to crash (dependant on ciphersuite). Both clients
   and servers are affected.

   This issue was reported to OpenSSL by Joe Orton (Red Hat).
   ([CVE-2017-3733])

   *Matt Caswell*

### Changes between 1.1.0c and 1.1.0d [26 Jan 2017]

 * Truncated packet could crash via OOB read

   If one side of an SSL/TLS path is running on a 32-bit host and a specific
   cipher is being used, then a truncated packet can cause that host to
   perform an out-of-bounds read, usually resulting in a crash.

   This issue was reported to OpenSSL by Robert Święcki of Google.
   ([CVE-2017-3731])

   *Andy Polyakov*

 * Bad (EC)DHE parameters cause a client crash

   If a malicious server supplies bad parameters for a DHE or ECDHE key
   exchange then this can result in the client attempting to dereference a
   NULL pointer leading to a client crash. This could be exploited in a Denial
   of Service attack.

   This issue was reported to OpenSSL by Guido Vranken.
   ([CVE-2017-3730])

   *Matt Caswell*

 * BN_mod_exp may produce incorrect results on x86_64

   There is a carry propagating bug in the x86_64 Montgomery squaring
   procedure. No EC algorithms are affected. Analysis suggests that attacks
   against RSA and DSA as a result of this defect would be very difficult to
   perform and are not believed likely. Attacks against DH are considered just
   feasible (although very difficult) because most of the work necessary to
   deduce information about a private key may be performed offline. The amount
   of resources required for such an attack would be very significant and
   likely only accessible to a limited number of attackers. An attacker would
   additionally need online access to an unpatched system using the target
   private key in a scenario with persistent DH parameters and a private
   key that is shared between multiple clients. For example this can occur by
   default in OpenSSL DHE based SSL/TLS ciphersuites. Note: This issue is very
   similar to CVE-2015-3193 but must be treated as a separate problem.

   This issue was reported to OpenSSL by the OSS-Fuzz project.
   ([CVE-2017-3732])

   *Andy Polyakov*

### Changes between 1.1.0b and 1.1.0c [10 Nov 2016]

 * ChaCha20/Poly1305 heap-buffer-overflow

   TLS connections using `*-CHACHA20-POLY1305` ciphersuites are susceptible to
   a DoS attack by corrupting larger payloads. This can result in an OpenSSL
   crash. This issue is not considered to be exploitable beyond a DoS.

   This issue was reported to OpenSSL by Robert Święcki (Google Security Team)
   ([CVE-2016-7054])

   *Richard Levitte*

 * CMS Null dereference

   Applications parsing invalid CMS structures can crash with a NULL pointer
   dereference. This is caused by a bug in the handling of the ASN.1 CHOICE
   type in OpenSSL 1.1.0 which can result in a NULL value being passed to the
   structure callback if an attempt is made to free certain invalid encodings.
   Only CHOICE structures using a callback which do not handle NULL value are
   affected.

   This issue was reported to OpenSSL by Tyler Nighswander of ForAllSecure.
   ([CVE-2016-7053])

   *Stephen Henson*

 * Montgomery multiplication may produce incorrect results

   There is a carry propagating bug in the Broadwell-specific Montgomery
   multiplication procedure that handles input lengths divisible by, but
   longer than 256 bits. Analysis suggests that attacks against RSA, DSA
   and DH private keys are impossible. This is because the subroutine in
   question is not used in operations with the private key itself and an input
   of the attacker's direct choice. Otherwise the bug can manifest itself as
   transient authentication and key negotiation failures or reproducible
   erroneous outcome of public-key operations with specially crafted input.
   Among EC algorithms only Brainpool P-512 curves are affected and one
   presumably can attack ECDH key negotiation. Impact was not analyzed in
   detail, because pre-requisites for attack are considered unlikely. Namely
   multiple clients have to choose the curve in question and the server has to
   share the private key among them, neither of which is default behaviour.
   Even then only clients that chose the curve will be affected.

   This issue was publicly reported as transient failures and was not
   initially recognized as a security issue. Thanks to Richard Morgan for
   providing reproducible case.
   ([CVE-2016-7055])

   *Andy Polyakov*

 * Removed automatic addition of RPATH in shared libraries and executables,
   as this was a remainder from OpenSSL 1.0.x and isn't needed any more.

   *Richard Levitte*

### Changes between 1.1.0a and 1.1.0b [26 Sep 2016]

 * Fix Use After Free for large message sizes

   The patch applied to address CVE-2016-6307 resulted in an issue where if a
   message larger than approx 16k is received then the underlying buffer to
   store the incoming message is reallocated and moved. Unfortunately a
   dangling pointer to the old location is left which results in an attempt to
   write to the previously freed location. This is likely to result in a
   crash, however it could potentially lead to execution of arbitrary code.

   This issue only affects OpenSSL 1.1.0a.

   This issue was reported to OpenSSL by Robert Święcki.
   ([CVE-2016-6309])

   *Matt Caswell*

### Changes between 1.1.0 and 1.1.0a [22 Sep 2016]

 * OCSP Status Request extension unbounded memory growth

   A malicious client can send an excessively large OCSP Status Request
   extension. If that client continually requests renegotiation, sending a
   large OCSP Status Request extension each time, then there will be unbounded
   memory growth on the server. This will eventually lead to a Denial Of
   Service attack through memory exhaustion. Servers with a default
   configuration are vulnerable even if they do not support OCSP. Builds using
   the "no-ocsp" build time option are not affected.

   This issue was reported to OpenSSL by Shi Lei (Gear Team, Qihoo 360 Inc.)
   ([CVE-2016-6304])

   *Matt Caswell*

 * SSL_peek() hang on empty record

   OpenSSL 1.1.0 SSL/TLS will hang during a call to SSL_peek() if the peer
   sends an empty record. This could be exploited by a malicious peer in a
   Denial Of Service attack.

   This issue was reported to OpenSSL by Alex Gaynor.
   ([CVE-2016-6305])

   *Matt Caswell*

 * Excessive allocation of memory in tls_get_message_header() and
   dtls1_preprocess_fragment()

   A (D)TLS message includes 3 bytes for its length in the header for the
   message. This would allow for messages up to 16Mb in length. Messages of
   this length are excessive and OpenSSL includes a check to ensure that a
   peer is sending reasonably sized messages in order to avoid too much memory
   being consumed to service a connection. A flaw in the logic of version
   1.1.0 means that memory for the message is allocated too early, prior to
   the excessive message length check. Due to way memory is allocated in
   OpenSSL this could mean an attacker could force up to 21Mb to be allocated
   to service a connection. This could lead to a Denial of Service through
   memory exhaustion. However, the excessive message length check still takes
   place, and this would cause the connection to immediately fail. Assuming
   that the application calls SSL_free() on the failed connection in a timely
   manner then the 21Mb of allocated memory will then be immediately freed
   again. Therefore the excessive memory allocation will be transitory in
   nature. This then means that there is only a security impact if:

   1) The application does not call SSL_free() in a timely manner in the event
   that the connection fails
   or
   2) The application is working in a constrained environment where there is
   very little free memory
   or
   3) The attacker initiates multiple connection attempts such that there are
   multiple connections in a state where memory has been allocated for the
   connection; SSL_free() has not yet been called; and there is insufficient
   memory to service the multiple requests.

   Except in the instance of (1) above any Denial Of Service is likely to be
   transitory because as soon as the connection fails the memory is
   subsequently freed again in the SSL_free() call. However there is an
   increased risk during this period of application crashes due to the lack of
   memory - which would then mean a more serious Denial of Service.

   This issue was reported to OpenSSL by Shi Lei (Gear Team, Qihoo 360 Inc.)
   (CVE-2016-6307 and CVE-2016-6308)

   *Matt Caswell*

 * solaris-x86-cc, i.e. 32-bit configuration with vendor compiler,
   had to be removed. Primary reason is that vendor assembler can't
   assemble our modules with -KPIC flag. As result it, assembly
   support, was not even available as option. But its lack means
   lack of side-channel resistant code, which is incompatible with
   security by todays standards. Fortunately gcc is readily available
   prepackaged option, which we firmly point at...

   *Andy Polyakov*

### Changes between 1.0.2h and 1.1.0  [25 Aug 2016]

 * Windows command-line tool supports UTF-8 opt-in option for arguments
   and console input. Setting OPENSSL_WIN32_UTF8 environment variable
   (to any value) allows Windows user to access PKCS#12 file generated
   with Windows CryptoAPI and protected with non-ASCII password, as well
   as files generated under UTF-8 locale on Linux also protected with
   non-ASCII password.

   *Andy Polyakov*

 * To mitigate the SWEET32 attack ([CVE-2016-2183]), 3DES cipher suites
   have been disabled by default and removed from DEFAULT, just like RC4.
   See the RC4 item below to re-enable both.

   *Rich Salz*

 * The method for finding the storage location for the Windows RAND seed file
   has changed. First we check %RANDFILE%. If that is not set then we check
   the directories %HOME%, %USERPROFILE% and %SYSTEMROOT% in that order. If
   all else fails we fall back to C:\.

   *Matt Caswell*

 * The EVP_EncryptUpdate() function has had its return type changed from void
   to int. A return of 0 indicates and error while a return of 1 indicates
   success.

   *Matt Caswell*

 * The flags RSA_FLAG_NO_CONSTTIME, DSA_FLAG_NO_EXP_CONSTTIME and
   DH_FLAG_NO_EXP_CONSTTIME which previously provided the ability to switch
   off the constant time implementation for RSA, DSA and DH have been made
   no-ops and deprecated.

   *Matt Caswell*

 * Windows RAND implementation was simplified to only get entropy by
   calling CryptGenRandom(). Various other RAND-related tickets
   were also closed.

   *Joseph Wylie Yandle, Rich Salz*

 * The stack and lhash API's were renamed to start with `OPENSSL_SK_`
   and `OPENSSL_LH_`, respectively.  The old names are available
   with API compatibility.  They new names are now completely documented.

   *Rich Salz*

 * Unify TYPE_up_ref(obj) methods signature.
   SSL_CTX_up_ref(), SSL_up_ref(), X509_up_ref(), EVP_PKEY_up_ref(),
   X509_CRL_up_ref(), X509_OBJECT_up_ref_count() methods are now returning an
   int (instead of void) like all others TYPE_up_ref() methods.
   So now these methods also check the return value of CRYPTO_atomic_add(),
   and the validity of object reference counter.

   *fdasilvayy@gmail.com*

 * With Windows Visual Studio builds, the .pdb files are installed
   alongside the installed libraries and executables.  For a static
   library installation, ossl_static.pdb is the associate compiler
   generated .pdb file to be used when linking programs.

   *Richard Levitte*

 * Remove openssl.spec.  Packaging files belong with the packagers.

   *Richard Levitte*

 * Automatic Darwin/OSX configuration has had a refresh, it will now
   recognise x86_64 architectures automatically.  You can still decide
   to build for a different bitness with the environment variable
   KERNEL_BITS (can be 32 or 64), for example:

           KERNEL_BITS=32 ./config

   *Richard Levitte*

 * Change default algorithms in pkcs8 utility to use PKCS#5 v2.0,
   256 bit AES and HMAC with SHA256.

   *Steve Henson*

 * Remove support for MIPS o32 ABI on IRIX (and IRIX only).

   *Andy Polyakov*

 * Triple-DES ciphers have been moved from HIGH to MEDIUM.

   *Rich Salz*

 * To enable users to have their own config files and build file templates,
   Configure looks in the directory indicated by the environment variable
   OPENSSL_LOCAL_CONFIG_DIR as well as the in-source Configurations/
   directory.  On VMS, OPENSSL_LOCAL_CONFIG_DIR is expected to be a logical
   name and is used as is.

   *Richard Levitte*

 * The following datatypes were made opaque: X509_OBJECT, X509_STORE_CTX,
   X509_STORE, X509_LOOKUP, and X509_LOOKUP_METHOD.  The unused type
   X509_CERT_FILE_CTX was removed.

   *Rich Salz*

 * "shared" builds are now the default. To create only static libraries use
   the "no-shared" Configure option.

   *Matt Caswell*

 * Remove the no-aes, no-hmac, no-rsa, no-sha and no-md5 Configure options.
   All of these option have not worked for some while and are fundamental
   algorithms.

   *Matt Caswell*

 * Make various cleanup routines no-ops and mark them as deprecated. Most
   global cleanup functions are no longer required because they are handled
   via auto-deinit (see OPENSSL_init_crypto and OPENSSL_init_ssl man pages).
   Explicitly de-initing can cause problems (e.g. where a library that uses
   OpenSSL de-inits, but an application is still using it). The affected
   functions are CONF_modules_free(), ENGINE_cleanup(), OBJ_cleanup(),
   EVP_cleanup(), BIO_sock_cleanup(), CRYPTO_cleanup_all_ex_data(),
   RAND_cleanup(), SSL_COMP_free_compression_methods(), ERR_free_strings() and
   COMP_zlib_cleanup().

   *Matt Caswell*

 * --strict-warnings no longer enables runtime debugging options
   such as REF_DEBUG. Instead, debug options are automatically
   enabled with '--debug' builds.

   *Andy Polyakov, Emilia Käsper*

 * Made DH and DH_METHOD opaque. The structures for managing DH objects
   have been moved out of the public header files. New functions for managing
   these have been added.

   *Matt Caswell*

 * Made RSA and RSA_METHOD opaque. The structures for managing RSA
   objects have been moved out of the public header files. New
   functions for managing these have been added.

   *Richard Levitte*

 * Made DSA and DSA_METHOD opaque. The structures for managing DSA objects
   have been moved out of the public header files. New functions for managing
   these have been added.

   *Matt Caswell*

 * Made BIO and BIO_METHOD opaque. The structures for managing BIOs have been
   moved out of the public header files. New functions for managing these
   have been added.

   *Matt Caswell*

 * Removed no-rijndael as a config option. Rijndael is an old name for AES.

   *Matt Caswell*

 * Removed the mk1mf build scripts.

   *Richard Levitte*

 * Headers are now wrapped, if necessary, with OPENSSL_NO_xxx, so
   it is always safe to #include a header now.

   *Rich Salz*

 * Removed the aged BC-32 config and all its supporting scripts

   *Richard Levitte*

 * Removed support for Ultrix, Netware, and OS/2.

   *Rich Salz*

 * Add support for HKDF.

   *Alessandro Ghedini*

 * Add support for blake2b and blake2s

   *Bill Cox*

 * Added support for "pipelining". Ciphers that have the
   EVP_CIPH_FLAG_PIPELINE flag set have a capability to process multiple
   encryptions/decryptions simultaneously. There are currently no built-in
   ciphers with this property but the expectation is that engines will be able
   to offer it to significantly improve throughput. Support has been extended
   into libssl so that multiple records for a single connection can be
   processed in one go (for >=TLS 1.1).

   *Matt Caswell*

 * Added the AFALG engine. This is an async capable engine which is able to
   offload work to the Linux kernel. In this initial version it only supports
   AES128-CBC. The kernel must be version 4.1.0 or greater.

   *Catriona Lucey*

 * OpenSSL now uses a new threading API. It is no longer necessary to
   set locking callbacks to use OpenSSL in a multi-threaded environment. There
   are two supported threading models: pthreads and windows threads. It is
   also possible to configure OpenSSL at compile time for "no-threads". The
   old threading API should no longer be used. The functions have been
   replaced with "no-op" compatibility macros.

   *Alessandro Ghedini, Matt Caswell*

 * Modify behavior of ALPN to invoke callback after SNI/servername
   callback, such that updates to the SSL_CTX affect ALPN.

   *Todd Short*

 * Add SSL_CIPHER queries for authentication and key-exchange.

   *Todd Short*

 * Changes to the DEFAULT cipherlist:
   - Prefer (EC)DHE handshakes over plain RSA.
   - Prefer AEAD ciphers over legacy ciphers.
   - Prefer ECDSA over RSA when both certificates are available.
   - Prefer TLSv1.2 ciphers/PRF.
   - Remove DSS, SEED, IDEA, CAMELLIA, and AES-CCM from the
     default cipherlist.

   *Emilia Käsper*

 * Change the ECC default curve list to be this, in order: x25519,
   secp256r1, secp521r1, secp384r1.

   *Rich Salz*

 * RC4 based libssl ciphersuites are now classed as "weak" ciphers and are
   disabled by default. They can be re-enabled using the
   enable-weak-ssl-ciphers option to Configure.

   *Matt Caswell*

 * If the server has ALPN configured, but supports no protocols that the
   client advertises, send a fatal "no_application_protocol" alert.
   This behaviour is SHALL in RFC 7301, though it isn't universally
   implemented by other servers.

   *Emilia Käsper*

 * Add X25519 support.
   Add ASN.1 and EVP_PKEY methods for X25519. This includes support
   for public and private key encoding using the format documented in
   draft-ietf-curdle-pkix-02. The corresponding EVP_PKEY method supports
   key generation and key derivation.

   TLS support complies with draft-ietf-tls-rfc4492bis-08 and uses
   X25519(29).

   *Steve Henson*

 * Deprecate SRP_VBASE_get_by_user.
   SRP_VBASE_get_by_user had inconsistent memory management behaviour.
   In order to fix an unavoidable memory leak ([CVE-2016-0798]),
   SRP_VBASE_get_by_user was changed to ignore the "fake user" SRP
   seed, even if the seed is configured.

   Users should use SRP_VBASE_get1_by_user instead. Note that in
   SRP_VBASE_get1_by_user, caller must free the returned value. Note
   also that even though configuring the SRP seed attempts to hide
   invalid usernames by continuing the handshake with fake
   credentials, this behaviour is not constant time and no strong
   guarantees are made that the handshake is indistinguishable from
   that of a valid user.

   *Emilia Käsper*

 * Configuration change; it's now possible to build dynamic engines
   without having to build shared libraries and vice versa.  This
   only applies to the engines in `engines/`, those in `crypto/engine/`
   will always be built into libcrypto (i.e. "static").

   Building dynamic engines is enabled by default; to disable, use
   the configuration option "disable-dynamic-engine".

   The only requirements for building dynamic engines are the
   presence of the DSO module and building with position independent
   code, so they will also automatically be disabled if configuring
   with "disable-dso" or "disable-pic".

   The macros OPENSSL_NO_STATIC_ENGINE and OPENSSL_NO_DYNAMIC_ENGINE
   are also taken away from openssl/opensslconf.h, as they are
   irrelevant.

   *Richard Levitte*

 * Configuration change; if there is a known flag to compile
   position independent code, it will always be applied on the
   libcrypto and libssl object files, and never on the application
   object files.  This means other libraries that use routines from
   libcrypto / libssl can be made into shared libraries regardless
   of how OpenSSL was configured.

   If this isn't desirable, the configuration options "disable-pic"
   or "no-pic" can be used to disable the use of PIC.  This will
   also disable building shared libraries and dynamic engines.

   *Richard Levitte*

 * Removed JPAKE code.  It was experimental and has no wide use.

   *Rich Salz*

 * The INSTALL_PREFIX Makefile variable has been renamed to
   DESTDIR.  That makes for less confusion on what this variable
   is for.  Also, the configuration option --install_prefix is
   removed.

   *Richard Levitte*

 * Heartbeat for TLS has been removed and is disabled by default
   for DTLS; configure with enable-heartbeats.  Code that uses the
   old #define's might need to be updated.

   *Emilia Käsper, Rich Salz*

 * Rename REF_CHECK to REF_DEBUG.

   *Rich Salz*

 * New "unified" build system

   The "unified" build system is aimed to be a common system for all
   platforms we support.  With it comes new support for VMS.

   This system builds supports building in a different directory tree
   than the source tree.  It produces one Makefile (for unix family
   or lookalikes), or one descrip.mms (for VMS).

   The source of information to make the Makefile / descrip.mms is
   small files called 'build.info', holding the necessary
   information for each directory with source to compile, and a
   template in Configurations, like unix-Makefile.tmpl or
   descrip.mms.tmpl.

   With this change, the library names were also renamed on Windows
   and on VMS.  They now have names that are closer to the standard
   on Unix, and include the major version number, and in certain
   cases, the architecture they are built for.  See "Notes on shared
   libraries" in INSTALL.

   We rely heavily on the perl module Text::Template.

   *Richard Levitte*

 * Added support for auto-initialisation and de-initialisation of the library.
   OpenSSL no longer requires explicit init or deinit routines to be called,
   except in certain circumstances. See the OPENSSL_init_crypto() and
   OPENSSL_init_ssl() man pages for further information.

   *Matt Caswell*

 * The arguments to the DTLSv1_listen function have changed. Specifically the
   "peer" argument is now expected to be a BIO_ADDR object.

 * Rewrite of BIO networking library. The BIO library lacked consistent
   support of IPv6, and adding it required some more extensive
   modifications.  This introduces the BIO_ADDR and BIO_ADDRINFO types,
   which hold all types of addresses and chains of address information.
   It also introduces a new API, with functions like BIO_socket,
   BIO_connect, BIO_listen, BIO_lookup and a rewrite of BIO_accept.
   The source/sink BIOs BIO_s_connect, BIO_s_accept and BIO_s_datagram
   have been adapted accordingly.

   *Richard Levitte*

 * RSA_padding_check_PKCS1_type_1 now accepts inputs with and without
   the leading 0-byte.

   *Emilia Käsper*

 * CRIME protection: disable compression by default, even if OpenSSL is
   compiled with zlib enabled. Applications can still enable compression
   by calling SSL_CTX_clear_options(ctx, SSL_OP_NO_COMPRESSION), or by
   using the SSL_CONF library to configure compression.

   *Emilia Käsper*

 * The signature of the session callback configured with
   SSL_CTX_sess_set_get_cb was changed. The read-only input buffer
   was explicitly marked as `const unsigned char*` instead of
   `unsigned char*`.

   *Emilia Käsper*

 * Always DPURIFY. Remove the use of uninitialized memory in the
   RNG, and other conditional uses of DPURIFY. This makes -DPURIFY a no-op.

   *Emilia Käsper*

 * Removed many obsolete configuration items, including
      DES_PTR, DES_RISC1, DES_RISC2, DES_INT
      MD2_CHAR, MD2_INT, MD2_LONG
      BF_PTR, BF_PTR2
      IDEA_SHORT, IDEA_LONG
      RC2_SHORT, RC2_LONG, RC4_LONG, RC4_CHUNK, RC4_INDEX

   *Rich Salz, with advice from Andy Polyakov*

 * Many BN internals have been moved to an internal header file.

   *Rich Salz with help from Andy Polyakov*

 * Configuration and writing out the results from it has changed.
   Files such as Makefile include/openssl/opensslconf.h and are now
   produced through general templates, such as Makefile.in and
   crypto/opensslconf.h.in and some help from the perl module
   Text::Template.

   Also, the center of configuration information is no longer
   Makefile.  Instead, Configure produces a perl module in
   configdata.pm which holds most of the config data (in the hash
   table %config), the target data that comes from the target
   configuration in one of the `Configurations/*.conf` files (in
   %target).

   *Richard Levitte*

 * To clarify their intended purposes, the Configure options
   --prefix and --openssldir change their semantics, and become more
   straightforward and less interdependent.

   --prefix shall be used exclusively to give the location INSTALLTOP
   where programs, scripts, libraries, include files and manuals are
   going to be installed.  The default is now /usr/local.

   --openssldir shall be used exclusively to give the default
   location OPENSSLDIR where certificates, private keys, CRLs are
   managed.  This is also where the default openssl.cnf gets
   installed.
   If the directory given with this option is a relative path, the
   values of both the --prefix value and the --openssldir value will
   be combined to become OPENSSLDIR.
   The default for --openssldir is INSTALLTOP/ssl.

   Anyone who uses --openssldir to specify where OpenSSL is to be
   installed MUST change to use --prefix instead.

   *Richard Levitte*

 * The GOST engine was out of date and therefore it has been removed. An up
   to date GOST engine is now being maintained in an external repository.
   See: <https://wiki.openssl.org/index.php/Binaries>. Libssl still retains
   support for GOST ciphersuites (these are only activated if a GOST engine
   is present).

   *Matt Caswell*

 * EGD is no longer supported by default; use enable-egd when
   configuring.

   *Ben Kaduk and Rich Salz*

 * The distribution now has Makefile.in files, which are used to
   create Makefile's when Configure is run.  *Configure must be run
   before trying to build now.*

   *Rich Salz*

 * The return value for SSL_CIPHER_description() for error conditions
   has changed.

   *Rich Salz*

 * Support for RFC6698/RFC7671 DANE TLSA peer authentication.

   Obtaining and performing DNSSEC validation of TLSA records is
   the application's responsibility.  The application provides
   the TLSA records of its choice to OpenSSL, and these are then
   used to authenticate the peer.

   The TLSA records need not even come from DNS.  They can, for
   example, be used to implement local end-entity certificate or
   trust-anchor "pinning", where the "pin" data takes the form
   of TLSA records, which can augment or replace verification
   based on the usual WebPKI public certification authorities.

   *Viktor Dukhovni*

 * Revert default OPENSSL_NO_DEPRECATED setting.  Instead OpenSSL
   continues to support deprecated interfaces in default builds.
   However, applications are strongly advised to compile their
   source files with -DOPENSSL_API_COMPAT=0x10100000L, which hides
   the declarations of all interfaces deprecated in 0.9.8, 1.0.0
   or the 1.1.0 releases.

   In environments in which all applications have been ported to
   not use any deprecated interfaces OpenSSL's Configure script
   should be used with the --api=1.1.0 option to entirely remove
   support for the deprecated features from the library and
   unconditionally disable them in the installed headers.
   Essentially the same effect can be achieved with the "no-deprecated"
   argument to Configure, except that this will always restrict
   the build to just the latest API, rather than a fixed API
   version.

   As applications are ported to future revisions of the API,
   they should update their compile-time OPENSSL_API_COMPAT define
   accordingly, but in most cases should be able to continue to
   compile with later releases.

   The OPENSSL_API_COMPAT versions for 1.0.0, and 0.9.8 are
   0x10000000L and 0x00908000L, respectively.  However those
   versions did not support the OPENSSL_API_COMPAT feature, and
   so applications are not typically tested for explicit support
   of just the undeprecated features of either release.

   *Viktor Dukhovni*

 * Add support for setting the minimum and maximum supported protocol.
   It can bet set via the SSL_set_min_proto_version() and
   SSL_set_max_proto_version(), or via the SSL_CONF's MinProtocol and
   MaxProtocol.  It's recommended to use the new APIs to disable
   protocols instead of disabling individual protocols using
   SSL_set_options() or SSL_CONF's Protocol.  This change also
   removes support for disabling TLS 1.2 in the OpenSSL TLS
   client at compile time by defining OPENSSL_NO_TLS1_2_CLIENT.

   *Kurt Roeckx*

 * Support for ChaCha20 and Poly1305 added to libcrypto and libssl.

   *Andy Polyakov*

 * New EC_KEY_METHOD, this replaces the older ECDSA_METHOD and ECDH_METHOD
   and integrates ECDSA and ECDH functionality into EC. Implementations can
   now redirect key generation and no longer need to convert to or from
   ECDSA_SIG format.

   Note: the ecdsa.h and ecdh.h headers are now no longer needed and just
   include the ec.h header file instead.

   *Steve Henson*

 * Remove support for all 40 and 56 bit ciphers.  This includes all the export
   ciphers who are no longer supported and drops support the ephemeral RSA key
   exchange. The LOW ciphers currently doesn't have any ciphers in it.

   *Kurt Roeckx*

 * Made EVP_MD_CTX, EVP_MD, EVP_CIPHER_CTX, EVP_CIPHER and HMAC_CTX
   opaque.  For HMAC_CTX, the following constructors and destructors
   were added:

       HMAC_CTX *HMAC_CTX_new(void);
       void HMAC_CTX_free(HMAC_CTX *ctx);

   For EVP_MD and EVP_CIPHER, complete APIs to create, fill and
   destroy such methods has been added.  See EVP_MD_meth_new(3) and
   EVP_CIPHER_meth_new(3) for documentation.

   Additional changes:
   1) `EVP_MD_CTX_cleanup()`, `EVP_CIPHER_CTX_cleanup()` and
      `HMAC_CTX_cleanup()` were removed. `HMAC_CTX_reset()` and
      `EVP_MD_CTX_reset()` should be called instead to reinitialise
      an already created structure.
   2) For consistency with the majority of our object creators and
      destructors, `EVP_MD_CTX_(create|destroy)` were renamed to
      `EVP_MD_CTX_(new|free)`.  The old names are retained as macros
      for deprecated builds.

   *Richard Levitte*

 * Added ASYNC support. Libcrypto now includes the async sub-library to enable
   cryptographic operations to be performed asynchronously as long as an
   asynchronous capable engine is used. See the ASYNC_start_job() man page for
   further details. Libssl has also had this capability integrated with the
   introduction of the new mode SSL_MODE_ASYNC and associated error
   SSL_ERROR_WANT_ASYNC. See the SSL_CTX_set_mode() and SSL_get_error() man
   pages. This work was developed in partnership with Intel Corp.

   *Matt Caswell*

 * SSL_{CTX_}set_ecdh_auto() has been removed and ECDH is support is
   always enabled now.  If you want to disable the support you should
   exclude it using the list of supported ciphers. This also means that the
   "-no_ecdhe" option has been removed from s_server.

   *Kurt Roeckx*

 * SSL_{CTX}_set_tmp_ecdh() which can set 1 EC curve now internally calls
   SSL_{CTX_}set1_curves() which can set a list.

   *Kurt Roeckx*

 * Remove support for SSL_{CTX_}set_tmp_ecdh_callback().  You should set the
   curve you want to support using SSL_{CTX_}set1_curves().

   *Kurt Roeckx*

 * State machine rewrite. The state machine code has been significantly
   refactored in order to remove much duplication of code and solve issues
   with the old code (see [ssl/statem/README.md](ssl/statem/README.md) for
   further details). This change does have some associated API changes.
   Notably the SSL_state() function has been removed and replaced by
   SSL_get_state which now returns an "OSSL_HANDSHAKE_STATE" instead of an int.
   SSL_set_state() has been removed altogether. The previous handshake states
   defined in ssl.h and ssl3.h have also been removed.

   *Matt Caswell*

 * All instances of the string "ssleay" in the public API were replaced
   with OpenSSL (case-matching; e.g., OPENSSL_VERSION for #define's)
   Some error codes related to internal RSA_eay API's were renamed.

   *Rich Salz*

 * The demo files in crypto/threads were moved to demo/threads.

   *Rich Salz*

 * Removed obsolete engines: 4758cca, aep, atalla, cswift, nuron, gmp,
   sureware and ubsec.

   *Matt Caswell, Rich Salz*

 * New ASN.1 embed macro.

   New ASN.1 macro ASN1_EMBED. This is the same as ASN1_SIMPLE except the
   structure is not allocated: it is part of the parent. That is instead of

           FOO *x;

   it must be:

           FOO x;

   This reduces memory fragmentation and make it impossible to accidentally
   set a mandatory field to NULL.

   This currently only works for some fields specifically a SEQUENCE, CHOICE,
   or ASN1_STRING type which is part of a parent SEQUENCE. Since it is
   equivalent to ASN1_SIMPLE it cannot be tagged, OPTIONAL, SET OF or
   SEQUENCE OF.

   *Steve Henson*

 * Remove EVP_CHECK_DES_KEY, a compile-time option that never compiled.

   *Emilia Käsper*

 * Removed DES and RC4 ciphersuites from DEFAULT. Also removed RC2 although
   in 1.0.2 EXPORT was already removed and the only RC2 ciphersuite is also
   an EXPORT one. COMPLEMENTOFDEFAULT has been updated accordingly to add
   DES and RC4 ciphersuites.

   *Matt Caswell*

 * Rewrite EVP_DecodeUpdate (base64 decoding) to fix several bugs.
   This changes the decoding behaviour for some invalid messages,
   though the change is mostly in the more lenient direction, and
   legacy behaviour is preserved as much as possible.

   *Emilia Käsper*

 * Fix no-stdio build.
   *David Woodhouse <David.Woodhouse@intel.com> and also*
   *Ivan Nestlerode <ivan.nestlerode@sonos.com>*

 * New testing framework
   The testing framework has been largely rewritten and is now using
   perl and the perl modules Test::Harness and an extended variant of
   Test::More called OpenSSL::Test to do its work.  All test scripts in
   test/ have been rewritten into test recipes, and all direct calls to
   executables in test/Makefile have become individual recipes using the
   simplified testing OpenSSL::Test::Simple.

   For documentation on our testing modules, do:

           perldoc test/testlib/OpenSSL/Test/Simple.pm
           perldoc test/testlib/OpenSSL/Test.pm

   *Richard Levitte*

 * Revamped memory debug; only -DCRYPTO_MDEBUG and -DCRYPTO_MDEBUG_ABORT
   are used; the latter aborts on memory leaks (usually checked on exit).
   Some undocumented "set malloc, etc., hooks" functions were removed
   and others were changed.  All are now documented.

   *Rich Salz*

 * In DSA_generate_parameters_ex, if the provided seed is too short,
   return an error

   *Rich Salz and Ismo Puustinen <ismo.puustinen@intel.com>*

 * Rewrite PSK to support ECDHE_PSK, DHE_PSK and RSA_PSK. Add ciphersuites
   from RFC4279, RFC4785, RFC5487, RFC5489.

   Thanks to Christian J. Dietrich and Giuseppe D'Angelo for the
   original RSA_PSK patch.

   *Steve Henson*

 * Dropped support for the SSL3_FLAGS_DELAY_CLIENT_FINISHED flag. This SSLeay
   era flag was never set throughout the codebase (only read). Also removed
   SSL3_FLAGS_POP_BUFFER which was only used if
   SSL3_FLAGS_DELAY_CLIENT_FINISHED was also set.

   *Matt Caswell*

 * Changed the default name options in the "ca", "crl", "req" and "x509"
   to be "oneline" instead of "compat".

   *Richard Levitte*

 * Remove SSL_OP_TLS_BLOCK_PADDING_BUG. This is SSLeay legacy, we're
   not aware of clients that still exhibit this bug, and the workaround
   hasn't been working properly for a while.

   *Emilia Käsper*

 * The return type of BIO_number_read() and BIO_number_written() as well as
   the corresponding num_read and num_write members in the BIO structure has
   changed from unsigned long to uint64_t. On platforms where an unsigned
   long is 32 bits (e.g. Windows) these counters could overflow if >4Gb is
   transferred.

   *Matt Caswell*

 * Given the pervasive nature of TLS extensions it is inadvisable to run
   OpenSSL without support for them. It also means that maintaining
   the OPENSSL_NO_TLSEXT option within the code is very invasive (and probably
   not well tested). Therefore the OPENSSL_NO_TLSEXT option has been removed.

   *Matt Caswell*

 * Removed support for the two export grade static DH ciphersuites
   EXP-DH-RSA-DES-CBC-SHA and EXP-DH-DSS-DES-CBC-SHA. These two ciphersuites
   were newly added (along with a number of other static DH ciphersuites) to
   1.0.2. However the two export ones have *never* worked since they were
   introduced. It seems strange in any case to be adding new export
   ciphersuites, and given "logjam" it also does not seem correct to fix them.

   *Matt Caswell*

 * Version negotiation has been rewritten. In particular SSLv23_method(),
   SSLv23_client_method() and SSLv23_server_method() have been deprecated,
   and turned into macros which simply call the new preferred function names
   TLS_method(), TLS_client_method() and TLS_server_method(). All new code
   should use the new names instead. Also as part of this change the ssl23.h
   header file has been removed.

   *Matt Caswell*

 * Support for Kerberos ciphersuites in TLS (RFC2712) has been removed. This
   code and the associated standard is no longer considered fit-for-purpose.

   *Matt Caswell*

 * RT2547 was closed.  When generating a private key, try to make the
   output file readable only by the owner.  This behavior change might
   be noticeable when interacting with other software.

 * Documented all exdata functions.  Added CRYPTO_free_ex_index.
   Added a test.

   *Rich Salz*

 * Added HTTP GET support to the ocsp command.

   *Rich Salz*

 * Changed default digest for the dgst and enc commands from MD5 to
   sha256

   *Rich Salz*

 * RAND_pseudo_bytes has been deprecated. Users should use RAND_bytes instead.

   *Matt Caswell*

 * Added support for TLS extended master secret from
   draft-ietf-tls-session-hash-03.txt. Thanks for Alfredo Pironti for an
   initial patch which was a great help during development.

   *Steve Henson*

 * All libssl internal structures have been removed from the public header
   files, and the OPENSSL_NO_SSL_INTERN option has been removed (since it is
   now redundant). Users should not attempt to access internal structures
   directly. Instead they should use the provided API functions.

   *Matt Caswell*

 * config has been changed so that by default OPENSSL_NO_DEPRECATED is used.
   Access to deprecated functions can be re-enabled by running config with
   "enable-deprecated". In addition applications wishing to use deprecated
   functions must define OPENSSL_USE_DEPRECATED. Note that this new behaviour
   will, by default, disable some transitive includes that previously existed
   in the header files (e.g. ec.h will no longer, by default, include bn.h)

   *Matt Caswell*

 * Added support for OCB mode. OpenSSL has been granted a patent license
   compatible with the OpenSSL license for use of OCB. Details are available
   at <https://www.openssl.org/source/OCB-patent-grant-OpenSSL.pdf>. Support
   for OCB can be removed by calling config with no-ocb.

   *Matt Caswell*

 * SSLv2 support has been removed.  It still supports receiving a SSLv2
   compatible client hello.

   *Kurt Roeckx*

 * Increased the minimal RSA keysize from 256 to 512 bits [Rich Salz],
   done while fixing the error code for the key-too-small case.

   *Annie Yousar <a.yousar@informatik.hu-berlin.de>*

 * CA.sh has been removed; use CA.pl instead.

   *Rich Salz*

 * Removed old DES API.

   *Rich Salz*

 * Remove various unsupported platforms:
      Sony NEWS4
      BEOS and BEOS_R5
      NeXT
      SUNOS
      MPE/iX
      Sinix/ReliantUNIX RM400
      DGUX
      NCR
      Tandem
      Cray
      16-bit platforms such as WIN16

   *Rich Salz*

 * Clean up OPENSSL_NO_xxx #define's
   - Use setbuf() and remove OPENSSL_NO_SETVBUF_IONBF
   - Rename OPENSSL_SYSNAME_xxx to OPENSSL_SYS_xxx
   - OPENSSL_NO_EC{DH,DSA} merged into OPENSSL_NO_EC
   - OPENSSL_NO_RIPEMD160, OPENSSL_NO_RIPEMD merged into OPENSSL_NO_RMD160
   - OPENSSL_NO_FP_API merged into OPENSSL_NO_STDIO
   - Remove OPENSSL_NO_BIO OPENSSL_NO_BUFFER OPENSSL_NO_CHAIN_VERIFY
     OPENSSL_NO_EVP OPENSSL_NO_FIPS_ERR OPENSSL_NO_HASH_COMP
     OPENSSL_NO_LHASH OPENSSL_NO_OBJECT OPENSSL_NO_SPEED OPENSSL_NO_STACK
     OPENSSL_NO_X509 OPENSSL_NO_X509_VERIFY
   - Remove MS_STATIC; it's a relic from platforms <32 bits.

   *Rich Salz*

 * Cleaned up dead code
     Remove all but one '#ifdef undef' which is to be looked at.

   *Rich Salz*

 * Clean up calling of xxx_free routines.
      Just like free(), fix most of the xxx_free routines to accept
      NULL.  Remove the non-null checks from callers.  Save much code.

   *Rich Salz*

 * Add secure heap for storage of private keys (when possible).
   Add BIO_s_secmem(), CBIGNUM, etc.
   Contributed by Akamai Technologies under our Corporate CLA.

   *Rich Salz*

 * Experimental support for a new, fast, unbiased prime candidate generator,
   bn_probable_prime_dh_coprime(). Not currently used by any prime generator.

   *Felix Laurie von Massenbach <felix@erbridge.co.uk>*

 * New output format NSS in the sess_id command line tool. This allows
   exporting the session id and the master key in NSS keylog format.

   *Martin Kaiser <martin@kaiser.cx>*

 * Harmonize version and its documentation. -f flag is used to display
   compilation flags.

   *mancha <mancha1@zoho.com>*

 * Fix eckey_priv_encode so it immediately returns an error upon a failure
   in i2d_ECPrivateKey.  Thanks to Ted Unangst for feedback on this issue.

   *mancha <mancha1@zoho.com>*

 * Fix some double frees. These are not thought to be exploitable.

   *mancha <mancha1@zoho.com>*

 * A missing bounds check in the handling of the TLS heartbeat extension
   can be used to reveal up to 64k of memory to a connected client or
   server.

   Thanks for Neel Mehta of Google Security for discovering this bug and to
   Adam Langley <agl@chromium.org> and Bodo Moeller <bmoeller@acm.org> for
   preparing the fix ([CVE-2014-0160])

   *Adam Langley, Bodo Moeller*

 * Fix for the attack described in the paper "Recovering OpenSSL
   ECDSA Nonces Using the FLUSH+RELOAD Cache Side-channel Attack"
   by Yuval Yarom and Naomi Benger. Details can be obtained from:
   <http://eprint.iacr.org/2014/140>

   Thanks to Yuval Yarom and Naomi Benger for discovering this
   flaw and to Yuval Yarom for supplying a fix ([CVE-2014-0076])

   *Yuval Yarom and Naomi Benger*

 * Use algorithm specific chains in SSL_CTX_use_certificate_chain_file():
   this fixes a limitation in previous versions of OpenSSL.

   *Steve Henson*

 * Experimental encrypt-then-mac support.

   Experimental support for encrypt then mac from
   draft-gutmann-tls-encrypt-then-mac-02.txt

   To enable it set the appropriate extension number (0x42 for the test
   server) using e.g. -DTLSEXT_TYPE_encrypt_then_mac=0x42

   For non-compliant peers (i.e. just about everything) this should have no
   effect.

   WARNING: EXPERIMENTAL, SUBJECT TO CHANGE.

   *Steve Henson*

 * Add EVP support for key wrapping algorithms, to avoid problems with
   existing code the flag EVP_CIPHER_CTX_WRAP_ALLOW has to be set in
   the EVP_CIPHER_CTX or an error is returned. Add AES and DES3 wrap
   algorithms and include tests cases.

   *Steve Henson*

 * Extend CMS code to support RSA-PSS signatures and RSA-OAEP for
   enveloped data.

   *Steve Henson*

 * Extended RSA OAEP support via EVP_PKEY API. Options to specify digest,
   MGF1 digest and OAEP label.

   *Steve Henson*

 * Make openssl verify return errors.

   *Chris Palmer <palmer@google.com> and Ben Laurie*

 * New function ASN1_TIME_diff to calculate the difference between two
   ASN1_TIME structures or one structure and the current time.

   *Steve Henson*

 * Update fips_test_suite to support multiple command line options. New
   test to induce all self test errors in sequence and check expected
   failures.

   *Steve Henson*

 * Add FIPS_{rsa,dsa,ecdsa}_{sign,verify} functions which digest and
   sign or verify all in one operation.

   *Steve Henson*

 * Add fips_algvs: a multicall fips utility incorporating all the algorithm
   test programs and fips_test_suite. Includes functionality to parse
   the minimal script output of fipsalgest.pl directly.

   *Steve Henson*

 * Add authorisation parameter to FIPS_module_mode_set().

   *Steve Henson*

 * Add FIPS selftest for ECDH algorithm using P-224 and B-233 curves.

   *Steve Henson*

 * Use separate DRBG fields for internal and external flags. New function
   FIPS_drbg_health_check() to perform on demand health checking. Add
   generation tests to fips_test_suite with reduced health check interval to
   demonstrate periodic health checking. Add "nodh" option to
   fips_test_suite to skip very slow DH test.

   *Steve Henson*

 * New function FIPS_get_cipherbynid() to lookup FIPS supported ciphers
   based on NID.

   *Steve Henson*

 * More extensive health check for DRBG checking many more failure modes.
   New function FIPS_selftest_drbg_all() to handle every possible DRBG
   combination: call this in fips_test_suite.

   *Steve Henson*

 * Add support for canonical generation of DSA parameter 'g'. See
   FIPS 186-3 A.2.3.

 * Add support for HMAC DRBG from SP800-90. Update DRBG algorithm test and
   POST to handle HMAC cases.

   *Steve Henson*

 * Add functions FIPS_module_version() and FIPS_module_version_text()
   to return numerical and string versions of the FIPS module number.

   *Steve Henson*

 * Rename FIPS_mode_set and FIPS_mode to FIPS_module_mode_set and
   FIPS_module_mode. FIPS_mode and FIPS_mode_set will be implemented
   outside the validated module in the FIPS capable OpenSSL.

   *Steve Henson*

 * Minor change to DRBG entropy callback semantics. In some cases
   there is no multiple of the block length between min_len and
   max_len. Allow the callback to return more than max_len bytes
   of entropy but discard any extra: it is the callback's responsibility
   to ensure that the extra data discarded does not impact the
   requested amount of entropy.

   *Steve Henson*

 * Add PRNG security strength checks to RSA, DSA and ECDSA using
   information in FIPS186-3, SP800-57 and SP800-131A.

   *Steve Henson*

 * CCM support via EVP. Interface is very similar to GCM case except we
   must supply all data in one chunk (i.e. no update, final) and the
   message length must be supplied if AAD is used. Add algorithm test
   support.

   *Steve Henson*

 * Initial version of POST overhaul. Add POST callback to allow the status
   of POST to be monitored and/or failures induced. Modify fips_test_suite
   to use callback. Always run all selftests even if one fails.

   *Steve Henson*

 * XTS support including algorithm test driver in the fips_gcmtest program.
   Note: this does increase the maximum key length from 32 to 64 bytes but
   there should be no binary compatibility issues as existing applications
   will never use XTS mode.

   *Steve Henson*

 * Extensive reorganisation of FIPS PRNG behaviour. Remove all dependencies
   to OpenSSL RAND code and replace with a tiny FIPS RAND API which also
   performs algorithm blocking for unapproved PRNG types. Also do not
   set PRNG type in FIPS_mode_set(): leave this to the application.
   Add default OpenSSL DRBG handling: sets up FIPS PRNG and seeds with
   the standard OpenSSL PRNG: set additional data to a date time vector.

   *Steve Henson*

 * Rename old X9.31 PRNG functions of the form `FIPS_rand*` to `FIPS_x931*`.
   This shouldn't present any incompatibility problems because applications
   shouldn't be using these directly and any that are will need to rethink
   anyway as the X9.31 PRNG is now deprecated by FIPS 140-2

   *Steve Henson*

 * Extensive self tests and health checking required by SP800-90 DRBG.
   Remove strength parameter from FIPS_drbg_instantiate and always
   instantiate at maximum supported strength.

   *Steve Henson*

 * Add ECDH code to fips module and fips_ecdhvs for primitives only testing.

   *Steve Henson*

 * New algorithm test program fips_dhvs to handle DH primitives only testing.

   *Steve Henson*

 * New function DH_compute_key_padded() to compute a DH key and pad with
   leading zeroes if needed: this complies with SP800-56A et al.

   *Steve Henson*

 * Initial implementation of SP800-90 DRBGs for Hash and CTR. Not used by
   anything, incomplete, subject to change and largely untested at present.

   *Steve Henson*

 * Modify fipscanisteronly build option to only build the necessary object
   files by filtering FIPS_EX_OBJ through a perl script in crypto/Makefile.

   *Steve Henson*

 * Add experimental option FIPSSYMS to give all symbols in
   fipscanister.o and FIPS or fips prefix. This will avoid
   conflicts with future versions of OpenSSL. Add perl script
   util/fipsas.pl to preprocess assembly language source files
   and rename any affected symbols.

   *Steve Henson*

 * Add selftest checks and algorithm block of non-fips algorithms in
   FIPS mode. Remove DES2 from selftests.

   *Steve Henson*

 * Add ECDSA code to fips module. Add tiny fips_ecdsa_check to just
   return internal method without any ENGINE dependencies. Add new
   tiny fips sign and verify functions.

   *Steve Henson*

 * New build option no-ec2m to disable characteristic 2 code.

   *Steve Henson*

 * New build option "fipscanisteronly". This only builds fipscanister.o
   and (currently) associated fips utilities. Uses the file Makefile.fips
   instead of Makefile.org as the prototype.

   *Steve Henson*

 * Add some FIPS mode restrictions to GCM. Add internal IV generator.
   Update fips_gcmtest to use IV generator.

   *Steve Henson*

 * Initial, experimental EVP support for AES-GCM. AAD can be input by
   setting output buffer to NULL. The `*Final` function must be
   called although it will not retrieve any additional data. The tag
   can be set or retrieved with a ctrl. The IV length is by default 12
   bytes (96 bits) but can be set to an alternative value. If the IV
   length exceeds the maximum IV length (currently 16 bytes) it cannot be
   set before the key.

   *Steve Henson*

 * New flag in ciphers: EVP_CIPH_FLAG_CUSTOM_CIPHER. This means the
   underlying do_cipher function handles all cipher semantics itself
   including padding and finalisation. This is useful if (for example)
   an ENGINE cipher handles block padding itself. The behaviour of
   do_cipher is subtly changed if this flag is set: the return value
   is the number of characters written to the output buffer (zero is
   no longer an error code) or a negative error code. Also if the
   input buffer is NULL and length 0 finalisation should be performed.

   *Steve Henson*

 * If a candidate issuer certificate is already part of the constructed
   path ignore it: new debug notification X509_V_ERR_PATH_LOOP for this case.

   *Steve Henson*

 * Improve forward-security support: add functions

           void SSL_CTX_set_not_resumable_session_callback(
                    SSL_CTX *ctx, int (*cb)(SSL *ssl, int is_forward_secure))
           void SSL_set_not_resumable_session_callback(
                    SSL *ssl, int (*cb)(SSL *ssl, int is_forward_secure))

   for use by SSL/TLS servers; the callback function will be called whenever a
   new session is created, and gets to decide whether the session may be
   cached to make it resumable (return 0) or not (return 1).  (As by the
   SSL/TLS protocol specifications, the session_id sent by the server will be
   empty to indicate that the session is not resumable; also, the server will
   not generate RFC 4507 (RFC 5077) session tickets.)

   A simple reasonable callback implementation is to return is_forward_secure.
   This parameter will be set to 1 or 0 depending on the ciphersuite selected
   by the SSL/TLS server library, indicating whether it can provide forward
   security.

   *Emilia Käsper <emilia.kasper@esat.kuleuven.be> (Google)*

 * New -verify_name option in command line utilities to set verification
   parameters by name.

   *Steve Henson*

 * Initial CMAC implementation. WARNING: EXPERIMENTAL, API MAY CHANGE.
   Add CMAC pkey methods.

   *Steve Henson*

 * Experimental renegotiation in s_server -www mode. If the client
   browses /reneg connection is renegotiated. If /renegcert it is
   renegotiated requesting a certificate.

   *Steve Henson*

 * Add an "external" session cache for debugging purposes to s_server. This
   should help trace issues which normally are only apparent in deployed
   multi-process servers.

   *Steve Henson*

 * Extensive audit of libcrypto with DEBUG_UNUSED. Fix many cases where
   return value is ignored. NB. The functions RAND_add(), RAND_seed(),
   BIO_set_cipher() and some obscure PEM functions were changed so they
   can now return an error. The RAND changes required a change to the
   RAND_METHOD structure.

   *Steve Henson*

 * New macro `__owur` for "OpenSSL Warn Unused Result". This makes use of
   a gcc attribute to warn if the result of a function is ignored. This
   is enable if DEBUG_UNUSED is set. Add to several functions in evp.h
   whose return value is often ignored.

   *Steve Henson*

 * New -noct, -requestct, -requirect and -ctlogfile options for s_client.
   These allow SCTs (signed certificate timestamps) to be requested and
   validated when establishing a connection.

   *Rob Percival <robpercival@google.com>*

OpenSSL 1.0.2
-------------

### Changes between 1.0.2s and 1.0.2t [10 Sep 2019]

 * For built-in EC curves, ensure an EC_GROUP built from the curve name is
   used even when parsing explicit parameters, when loading a encoded key
   or calling `EC_GROUP_new_from_ecpkparameters()`/
   `EC_GROUP_new_from_ecparameters()`.
   This prevents bypass of security hardening and performance gains,
   especially for curves with specialized EC_METHODs.
   By default, if a key encoded with explicit parameters is loaded and later
   encoded, the output is still encoded with explicit parameters, even if
   internally a "named" EC_GROUP is used for computation.

   *Nicola Tuveri*

 * Compute ECC cofactors if not provided during EC_GROUP construction. Before
   this change, EC_GROUP_set_generator would accept order and/or cofactor as
   NULL. After this change, only the cofactor parameter can be NULL. It also
   does some minimal sanity checks on the passed order.
   ([CVE-2019-1547])

   *Billy Bob Brumley*

 * Fixed a padding oracle in PKCS7_dataDecode and CMS_decrypt_set1_pkey.
   An attack is simple, if the first CMS_recipientInfo is valid but the
   second CMS_recipientInfo is chosen ciphertext. If the second
   recipientInfo decodes to PKCS #1 v1.5 form plaintext, the correct
   encryption key will be replaced by garbage, and the message cannot be
   decoded, but if the RSA decryption fails, the correct encryption key is
   used and the recipient will not notice the attack.
   As a work around for this potential attack the length of the decrypted
   key must be equal to the cipher default key length, in case the
   certifiate is not given and all recipientInfo are tried out.
   The old behaviour can be re-enabled in the CMS code by setting the
   CMS_DEBUG_DECRYPT flag.
   ([CVE-2019-1563])

   *Bernd Edlinger*

 * Document issue with installation paths in diverse Windows builds

   '/usr/local/ssl' is an unsafe prefix for location to install OpenSSL
   binaries and run-time config file.
   ([CVE-2019-1552])

   *Richard Levitte*

### Changes between 1.0.2r and 1.0.2s [28 May 2019]

 * Change the default RSA, DSA and DH size to 2048 bit instead of 1024.
   This changes the size when using the `genpkey` command when no size is given.
   It fixes an omission in earlier changes that changed all RSA, DSA and DH
   generation commands to use 2048 bits by default.

   *Kurt Roeckx*

 * Add FIPS support for Android Arm 64-bit

   Support for Android Arm 64-bit was added to the OpenSSL FIPS Object
   Module in Version 2.0.10. For some reason, the corresponding target
   'android64-aarch64' was missing OpenSSL 1.0.2, whence it could not be
   built with FIPS support on Android Arm 64-bit. This omission has been
   fixed.

   *Matthias St. Pierre*

### Changes between 1.0.2q and 1.0.2r [26 Feb 2019]

 * 0-byte record padding oracle

   If an application encounters a fatal protocol error and then calls
   SSL_shutdown() twice (once to send a close_notify, and once to receive one)
   then OpenSSL can respond differently to the calling application if a 0 byte
   record is received with invalid padding compared to if a 0 byte record is
   received with an invalid MAC. If the application then behaves differently
   based on that in a way that is detectable to the remote peer, then this
   amounts to a padding oracle that could be used to decrypt data.

   In order for this to be exploitable "non-stitched" ciphersuites must be in
   use. Stitched ciphersuites are optimised implementations of certain
   commonly used ciphersuites. Also the application must call SSL_shutdown()
   twice even if a protocol error has occurred (applications should not do
   this but some do anyway).

   This issue was discovered by Juraj Somorovsky, Robert Merget and Nimrod
   Aviram, with additional investigation by Steven Collison and Andrew
   Hourselt. It was reported to OpenSSL on 10th December 2018.
   ([CVE-2019-1559])

   *Matt Caswell*

 * Move strictness check from EVP_PKEY_asn1_new() to EVP_PKEY_asn1_add0().

   *Richard Levitte*

### Changes between 1.0.2p and 1.0.2q [20 Nov 2018]

 * Microarchitecture timing vulnerability in ECC scalar multiplication

   OpenSSL ECC scalar multiplication, used in e.g. ECDSA and ECDH, has been
   shown to be vulnerable to a microarchitecture timing side channel attack.
   An attacker with sufficient access to mount local timing attacks during
   ECDSA signature generation could recover the private key.

   This issue was reported to OpenSSL on 26th October 2018 by Alejandro
   Cabrera Aldaya, Billy Brumley, Sohaib ul Hassan, Cesar Pereida Garcia and
   Nicola Tuveri.
   ([CVE-2018-5407])

   *Billy Brumley*

 * Timing vulnerability in DSA signature generation

   The OpenSSL DSA signature algorithm has been shown to be vulnerable to a
   timing side channel attack. An attacker could use variations in the signing
   algorithm to recover the private key.

   This issue was reported to OpenSSL on 16th October 2018 by Samuel Weiser.
   ([CVE-2018-0734])

   *Paul Dale*

 * Resolve a compatibility issue in EC_GROUP handling with the FIPS Object
   Module, accidentally introduced while backporting security fixes from the
   development branch and hindering the use of ECC in FIPS mode.

   *Nicola Tuveri*

### Changes between 1.0.2o and 1.0.2p [14 Aug 2018]

 * Client DoS due to large DH parameter

   During key agreement in a TLS handshake using a DH(E) based ciphersuite a
   malicious server can send a very large prime value to the client. This will
   cause the client to spend an unreasonably long period of time generating a
   key for this prime resulting in a hang until the client has finished. This
   could be exploited in a Denial Of Service attack.

   This issue was reported to OpenSSL on 5th June 2018 by Guido Vranken
   ([CVE-2018-0732])

   *Guido Vranken*

 * Cache timing vulnerability in RSA Key Generation

   The OpenSSL RSA Key generation algorithm has been shown to be vulnerable to
   a cache timing side channel attack. An attacker with sufficient access to
   mount cache timing attacks during the RSA key generation process could
   recover the private key.

   This issue was reported to OpenSSL on 4th April 2018 by Alejandro Cabrera
   Aldaya, Billy Brumley, Cesar Pereida Garcia and Luis Manuel Alvarez Tapia.
   ([CVE-2018-0737])

   *Billy Brumley*

 * Make EVP_PKEY_asn1_new() a bit stricter about its input.  A NULL pem_str
   parameter is no longer accepted, as it leads to a corrupt table.  NULL
   pem_str is reserved for alias entries only.

   *Richard Levitte*

 * Revert blinding in ECDSA sign and instead make problematic addition
   length-invariant. Switch even to fixed-length Montgomery multiplication.

   *Andy Polyakov*

 * Change generating and checking of primes so that the error rate of not
   being prime depends on the intended use based on the size of the input.
   For larger primes this will result in more rounds of Miller-Rabin.
   The maximal error rate for primes with more than 1080 bits is lowered
   to 2^-128.

   *Kurt Roeckx, Annie Yousar*

 * Increase the number of Miller-Rabin rounds for DSA key generating to 64.

   *Kurt Roeckx*

 * Add blinding to ECDSA and DSA signatures to protect against side channel
   attacks discovered by Keegan Ryan (NCC Group).

   *Matt Caswell*

 * When unlocking a pass phrase protected PEM file or PKCS#8 container, we
   now allow empty (zero character) pass phrases.

   *Richard Levitte*

 * Certificate time validation (X509_cmp_time) enforces stricter
   compliance with RFC 5280. Fractional seconds and timezone offsets
   are no longer allowed.

   *Emilia Käsper*

### Changes between 1.0.2n and 1.0.2o [27 Mar 2018]

 * Constructed ASN.1 types with a recursive definition could exceed the stack

   Constructed ASN.1 types with a recursive definition (such as can be found
   in PKCS7) could eventually exceed the stack given malicious input with
   excessive recursion. This could result in a Denial Of Service attack. There
   are no such structures used within SSL/TLS that come from untrusted sources
   so this is considered safe.

   This issue was reported to OpenSSL on 4th January 2018 by the OSS-fuzz
   project.
   ([CVE-2018-0739])

   *Matt Caswell*

### Changes between 1.0.2m and 1.0.2n [7 Dec 2017]

 * Read/write after SSL object in error state

   OpenSSL 1.0.2 (starting from version 1.0.2b) introduced an "error state"
   mechanism. The intent was that if a fatal error occurred during a handshake
   then OpenSSL would move into the error state and would immediately fail if
   you attempted to continue the handshake. This works as designed for the
   explicit handshake functions (SSL_do_handshake(), SSL_accept() and
   SSL_connect()), however due to a bug it does not work correctly if
   SSL_read() or SSL_write() is called directly. In that scenario, if the
   handshake fails then a fatal error will be returned in the initial function
   call. If SSL_read()/SSL_write() is subsequently called by the application
   for the same SSL object then it will succeed and the data is passed without
   being decrypted/encrypted directly from the SSL/TLS record layer.

   In order to exploit this issue an application bug would have to be present
   that resulted in a call to SSL_read()/SSL_write() being issued after having
   already received a fatal error.

   This issue was reported to OpenSSL by David Benjamin (Google).
   ([CVE-2017-3737])

   *Matt Caswell*

 * rsaz_1024_mul_avx2 overflow bug on x86_64

   There is an overflow bug in the AVX2 Montgomery multiplication procedure
   used in exponentiation with 1024-bit moduli. No EC algorithms are affected.
   Analysis suggests that attacks against RSA and DSA as a result of this
   defect would be very difficult to perform and are not believed likely.
   Attacks against DH1024 are considered just feasible, because most of the
   work necessary to deduce information about a private key may be performed
   offline. The amount of resources required for such an attack would be
   significant. However, for an attack on TLS to be meaningful, the server
   would have to share the DH1024 private key among multiple clients, which is
   no longer an option since CVE-2016-0701.

   This only affects processors that support the AVX2 but not ADX extensions
   like Intel Haswell (4th generation).

   This issue was reported to OpenSSL by David Benjamin (Google). The issue
   was originally found via the OSS-Fuzz project.
   ([CVE-2017-3738])

   *Andy Polyakov*

### Changes between 1.0.2l and 1.0.2m [2 Nov 2017]

 * bn_sqrx8x_internal carry bug on x86_64

   There is a carry propagating bug in the x86_64 Montgomery squaring
   procedure. No EC algorithms are affected. Analysis suggests that attacks
   against RSA and DSA as a result of this defect would be very difficult to
   perform and are not believed likely. Attacks against DH are considered just
   feasible (although very difficult) because most of the work necessary to
   deduce information about a private key may be performed offline. The amount
   of resources required for such an attack would be very significant and
   likely only accessible to a limited number of attackers. An attacker would
   additionally need online access to an unpatched system using the target
   private key in a scenario with persistent DH parameters and a private
   key that is shared between multiple clients.

   This only affects processors that support the BMI1, BMI2 and ADX extensions
   like Intel Broadwell (5th generation) and later or AMD Ryzen.

   This issue was reported to OpenSSL by the OSS-Fuzz project.
   ([CVE-2017-3736])

   *Andy Polyakov*

 * Malformed X.509 IPAddressFamily could cause OOB read

   If an X.509 certificate has a malformed IPAddressFamily extension,
   OpenSSL could do a one-byte buffer overread. The most likely result
   would be an erroneous display of the certificate in text format.

   This issue was reported to OpenSSL by the OSS-Fuzz project.

   *Rich Salz*

### Changes between 1.0.2k and 1.0.2l [25 May 2017]

 * Have 'config' recognise 64-bit mingw and choose 'mingw64' as the target
   platform rather than 'mingw'.

   *Richard Levitte*

### Changes between 1.0.2j and 1.0.2k [26 Jan 2017]

 * Truncated packet could crash via OOB read

   If one side of an SSL/TLS path is running on a 32-bit host and a specific
   cipher is being used, then a truncated packet can cause that host to
   perform an out-of-bounds read, usually resulting in a crash.

   This issue was reported to OpenSSL by Robert Święcki of Google.
   ([CVE-2017-3731])

   *Andy Polyakov*

 * BN_mod_exp may produce incorrect results on x86_64

   There is a carry propagating bug in the x86_64 Montgomery squaring
   procedure. No EC algorithms are affected. Analysis suggests that attacks
   against RSA and DSA as a result of this defect would be very difficult to
   perform and are not believed likely. Attacks against DH are considered just
   feasible (although very difficult) because most of the work necessary to
   deduce information about a private key may be performed offline. The amount
   of resources required for such an attack would be very significant and
   likely only accessible to a limited number of attackers. An attacker would
   additionally need online access to an unpatched system using the target
   private key in a scenario with persistent DH parameters and a private
   key that is shared between multiple clients. For example this can occur by
   default in OpenSSL DHE based SSL/TLS ciphersuites. Note: This issue is very
   similar to CVE-2015-3193 but must be treated as a separate problem.

   This issue was reported to OpenSSL by the OSS-Fuzz project.
   ([CVE-2017-3732])

   *Andy Polyakov*

 * Montgomery multiplication may produce incorrect results

   There is a carry propagating bug in the Broadwell-specific Montgomery
   multiplication procedure that handles input lengths divisible by, but
   longer than 256 bits. Analysis suggests that attacks against RSA, DSA
   and DH private keys are impossible. This is because the subroutine in
   question is not used in operations with the private key itself and an input
   of the attacker's direct choice. Otherwise the bug can manifest itself as
   transient authentication and key negotiation failures or reproducible
   erroneous outcome of public-key operations with specially crafted input.
   Among EC algorithms only Brainpool P-512 curves are affected and one
   presumably can attack ECDH key negotiation. Impact was not analyzed in
   detail, because pre-requisites for attack are considered unlikely. Namely
   multiple clients have to choose the curve in question and the server has to
   share the private key among them, neither of which is default behaviour.
   Even then only clients that chose the curve will be affected.

   This issue was publicly reported as transient failures and was not
   initially recognized as a security issue. Thanks to Richard Morgan for
   providing reproducible case.
   ([CVE-2016-7055])

   *Andy Polyakov*

 * OpenSSL now fails if it receives an unrecognised record type in TLS1.0
   or TLS1.1. Previously this only happened in SSLv3 and TLS1.2. This is to
   prevent issues where no progress is being made and the peer continually
   sends unrecognised record types, using up resources processing them.

   *Matt Caswell*

### Changes between 1.0.2i and 1.0.2j [26 Sep 2016]

 * Missing CRL sanity check

   A bug fix which included a CRL sanity check was added to OpenSSL 1.1.0
   but was omitted from OpenSSL 1.0.2i. As a result any attempt to use
   CRLs in OpenSSL 1.0.2i will crash with a null pointer exception.

   This issue only affects the OpenSSL 1.0.2i
   ([CVE-2016-7052])

   *Matt Caswell*

### Changes between 1.0.2h and 1.0.2i [22 Sep 2016]

 * OCSP Status Request extension unbounded memory growth

   A malicious client can send an excessively large OCSP Status Request
   extension. If that client continually requests renegotiation, sending a
   large OCSP Status Request extension each time, then there will be unbounded
   memory growth on the server. This will eventually lead to a Denial Of
   Service attack through memory exhaustion. Servers with a default
   configuration are vulnerable even if they do not support OCSP. Builds using
   the "no-ocsp" build time option are not affected.

   This issue was reported to OpenSSL by Shi Lei (Gear Team, Qihoo 360 Inc.)
   ([CVE-2016-6304])

   *Matt Caswell*

 * In order to mitigate the SWEET32 attack, the DES ciphers were moved from
   HIGH to MEDIUM.

   This issue was reported to OpenSSL Karthikeyan Bhargavan and Gaetan
   Leurent (INRIA)
   ([CVE-2016-2183])

   *Rich Salz*

 * OOB write in MDC2_Update()

   An overflow can occur in MDC2_Update() either if called directly or
   through the EVP_DigestUpdate() function using MDC2. If an attacker
   is able to supply very large amounts of input data after a previous
   call to EVP_EncryptUpdate() with a partial block then a length check
   can overflow resulting in a heap corruption.

   The amount of data needed is comparable to SIZE_MAX which is impractical
   on most platforms.

   This issue was reported to OpenSSL by Shi Lei (Gear Team, Qihoo 360 Inc.)
   ([CVE-2016-6303])

   *Stephen Henson*

 * Malformed SHA512 ticket DoS

   If a server uses SHA512 for TLS session ticket HMAC it is vulnerable to a
   DoS attack where a malformed ticket will result in an OOB read which will
   ultimately crash.

   The use of SHA512 in TLS session tickets is comparatively rare as it requires
   a custom server callback and ticket lookup mechanism.

   This issue was reported to OpenSSL by Shi Lei (Gear Team, Qihoo 360 Inc.)
   ([CVE-2016-6302])

   *Stephen Henson*

 * OOB write in BN_bn2dec()

   The function BN_bn2dec() does not check the return value of BN_div_word().
   This can cause an OOB write if an application uses this function with an
   overly large BIGNUM. This could be a problem if an overly large certificate
   or CRL is printed out from an untrusted source. TLS is not affected because
   record limits will reject an oversized certificate before it is parsed.

   This issue was reported to OpenSSL by Shi Lei (Gear Team, Qihoo 360 Inc.)
   ([CVE-2016-2182])

   *Stephen Henson*

 * OOB read in TS_OBJ_print_bio()

   The function TS_OBJ_print_bio() misuses OBJ_obj2txt(): the return value is
   the total length the OID text representation would use and not the amount
   of data written. This will result in OOB reads when large OIDs are
   presented.

   This issue was reported to OpenSSL by Shi Lei (Gear Team, Qihoo 360 Inc.)
   ([CVE-2016-2180])

   *Stephen Henson*

 * Pointer arithmetic undefined behaviour

   Avoid some undefined pointer arithmetic

   A common idiom in the codebase is to check limits in the following manner:
   "p + len > limit"

   Where "p" points to some malloc'd data of SIZE bytes and
   limit == p + SIZE

   "len" here could be from some externally supplied data (e.g. from a TLS
   message).

   The rules of C pointer arithmetic are such that "p + len" is only well
   defined where len <= SIZE. Therefore the above idiom is actually
   undefined behaviour.

   For example this could cause problems if some malloc implementation
   provides an address for "p" such that "p + len" actually overflows for
   values of len that are too big and therefore p + len < limit.

   This issue was reported to OpenSSL by Guido Vranken
   ([CVE-2016-2177])

   *Matt Caswell*

 * Constant time flag not preserved in DSA signing

   Operations in the DSA signing algorithm should run in constant time in
   order to avoid side channel attacks. A flaw in the OpenSSL DSA
   implementation means that a non-constant time codepath is followed for
   certain operations. This has been demonstrated through a cache-timing
   attack to be sufficient for an attacker to recover the private DSA key.

   This issue was reported by César Pereida (Aalto University), Billy Brumley
   (Tampere University of Technology), and Yuval Yarom (The University of
   Adelaide and NICTA).
   ([CVE-2016-2178])

   *César Pereida*

 * DTLS buffered message DoS

   In a DTLS connection where handshake messages are delivered out-of-order
   those messages that OpenSSL is not yet ready to process will be buffered
   for later use. Under certain circumstances, a flaw in the logic means that
   those messages do not get removed from the buffer even though the handshake
   has been completed. An attacker could force up to approx. 15 messages to
   remain in the buffer when they are no longer required. These messages will
   be cleared when the DTLS connection is closed. The default maximum size for
   a message is 100k. Therefore the attacker could force an additional 1500k
   to be consumed per connection. By opening many simulataneous connections an
   attacker could cause a DoS attack through memory exhaustion.

   This issue was reported to OpenSSL by Quan Luo.
   ([CVE-2016-2179])

   *Matt Caswell*

 * DTLS replay protection DoS

   A flaw in the DTLS replay attack protection mechanism means that records
   that arrive for future epochs update the replay protection "window" before
   the MAC for the record has been validated. This could be exploited by an
   attacker by sending a record for the next epoch (which does not have to
   decrypt or have a valid MAC), with a very large sequence number. This means
   that all subsequent legitimate packets are dropped causing a denial of
   service for a specific DTLS connection.

   This issue was reported to OpenSSL by the OCAP audit team.
   ([CVE-2016-2181])

   *Matt Caswell*

 * Certificate message OOB reads

   In OpenSSL 1.0.2 and earlier some missing message length checks can result
   in OOB reads of up to 2 bytes beyond an allocated buffer. There is a
   theoretical DoS risk but this has not been observed in practice on common
   platforms.

   The messages affected are client certificate, client certificate request
   and server certificate. As a result the attack can only be performed
   against a client or a server which enables client authentication.

   This issue was reported to OpenSSL by Shi Lei (Gear Team, Qihoo 360 Inc.)
   ([CVE-2016-6306])

   *Stephen Henson*

### Changes between 1.0.2g and 1.0.2h [3 May 2016]

 * Prevent padding oracle in AES-NI CBC MAC check

   A MITM attacker can use a padding oracle attack to decrypt traffic
   when the connection uses an AES CBC cipher and the server support
   AES-NI.

   This issue was introduced as part of the fix for Lucky 13 padding
   attack ([CVE-2013-0169]). The padding check was rewritten to be in
   constant time by making sure that always the same bytes are read and
   compared against either the MAC or padding bytes. But it no longer
   checked that there was enough data to have both the MAC and padding
   bytes.

   This issue was reported by Juraj Somorovsky using TLS-Attacker.

   *Kurt Roeckx*

 * Fix EVP_EncodeUpdate overflow

   An overflow can occur in the EVP_EncodeUpdate() function which is used for
   Base64 encoding of binary data. If an attacker is able to supply very large
   amounts of input data then a length check can overflow resulting in a heap
   corruption.

   Internally to OpenSSL the EVP_EncodeUpdate() function is primarily used by
   the `PEM_write_bio*` family of functions. These are mainly used within the
   OpenSSL command line applications, so any application which processes data
   from an untrusted source and outputs it as a PEM file should be considered
   vulnerable to this issue. User applications that call these APIs directly
   with large amounts of untrusted data may also be vulnerable.

   This issue was reported by Guido Vranken.
   ([CVE-2016-2105])

   *Matt Caswell*

 * Fix EVP_EncryptUpdate overflow

   An overflow can occur in the EVP_EncryptUpdate() function. If an attacker
   is able to supply very large amounts of input data after a previous call to
   EVP_EncryptUpdate() with a partial block then a length check can overflow
   resulting in a heap corruption. Following an analysis of all OpenSSL
   internal usage of the EVP_EncryptUpdate() function all usage is one of two
   forms. The first form is where the EVP_EncryptUpdate() call is known to be
   the first called function after an EVP_EncryptInit(), and therefore that
   specific call must be safe. The second form is where the length passed to
   EVP_EncryptUpdate() can be seen from the code to be some small value and
   therefore there is no possibility of an overflow. Since all instances are
   one of these two forms, it is believed that there can be no overflows in
   internal code due to this problem. It should be noted that
   EVP_DecryptUpdate() can call EVP_EncryptUpdate() in certain code paths.
   Also EVP_CipherUpdate() is a synonym for EVP_EncryptUpdate(). All instances
   of these calls have also been analysed too and it is believed there are no
   instances in internal usage where an overflow could occur.

   This issue was reported by Guido Vranken.
   ([CVE-2016-2106])

   *Matt Caswell*

 * Prevent ASN.1 BIO excessive memory allocation

   When ASN.1 data is read from a BIO using functions such as d2i_CMS_bio()
   a short invalid encoding can cause allocation of large amounts of memory
   potentially consuming excessive resources or exhausting memory.

   Any application parsing untrusted data through d2i BIO functions is
   affected. The memory based functions such as d2i_X509() are *not* affected.
   Since the memory based functions are used by the TLS library, TLS
   applications are not affected.

   This issue was reported by Brian Carpenter.
   ([CVE-2016-2109])

   *Stephen Henson*

 * EBCDIC overread

   ASN1 Strings that are over 1024 bytes can cause an overread in applications
   using the X509_NAME_oneline() function on EBCDIC systems. This could result
   in arbitrary stack data being returned in the buffer.

   This issue was reported by Guido Vranken.
   ([CVE-2016-2176])

   *Matt Caswell*

 * Modify behavior of ALPN to invoke callback after SNI/servername
   callback, such that updates to the SSL_CTX affect ALPN.

   *Todd Short*

 * Remove LOW from the DEFAULT cipher list.  This removes singles DES from the
   default.

   *Kurt Roeckx*

 * Only remove the SSLv2 methods with the no-ssl2-method option. When the
   methods are enabled and ssl2 is disabled the methods return NULL.

   *Kurt Roeckx*

### Changes between 1.0.2f and 1.0.2g [1 Mar 2016]

* Disable weak ciphers in SSLv3 and up in default builds of OpenSSL.
  Builds that are not configured with "enable-weak-ssl-ciphers" will not
  provide any "EXPORT" or "LOW" strength ciphers.

  *Viktor Dukhovni*

* Disable SSLv2 default build, default negotiation and weak ciphers.  SSLv2
  is by default disabled at build-time.  Builds that are not configured with
  "enable-ssl2" will not support SSLv2.  Even if "enable-ssl2" is used,
  users who want to negotiate SSLv2 via the version-flexible SSLv23_method()
  will need to explicitly call either of:

      SSL_CTX_clear_options(ctx, SSL_OP_NO_SSLv2);
  or
      SSL_clear_options(ssl, SSL_OP_NO_SSLv2);

  as appropriate.  Even if either of those is used, or the application
  explicitly uses the version-specific SSLv2_method() or its client and
  server variants, SSLv2 ciphers vulnerable to exhaustive search key
  recovery have been removed.  Specifically, the SSLv2 40-bit EXPORT
  ciphers, and SSLv2 56-bit DES are no longer available.
  ([CVE-2016-0800])

   *Viktor Dukhovni*

 * Fix a double-free in DSA code

   A double free bug was discovered when OpenSSL parses malformed DSA private
   keys and could lead to a DoS attack or memory corruption for applications
   that receive DSA private keys from untrusted sources.  This scenario is
   considered rare.

   This issue was reported to OpenSSL by Adam Langley(Google/BoringSSL) using
   libFuzzer.
   ([CVE-2016-0705])

   *Stephen Henson*

 * Disable SRP fake user seed to address a server memory leak.

   Add a new method SRP_VBASE_get1_by_user that handles the seed properly.

   SRP_VBASE_get_by_user had inconsistent memory management behaviour.
   In order to fix an unavoidable memory leak, SRP_VBASE_get_by_user
   was changed to ignore the "fake user" SRP seed, even if the seed
   is configured.

   Users should use SRP_VBASE_get1_by_user instead. Note that in
   SRP_VBASE_get1_by_user, caller must free the returned value. Note
   also that even though configuring the SRP seed attempts to hide
   invalid usernames by continuing the handshake with fake
   credentials, this behaviour is not constant time and no strong
   guarantees are made that the handshake is indistinguishable from
   that of a valid user.
   ([CVE-2016-0798])

   *Emilia Käsper*

 * Fix BN_hex2bn/BN_dec2bn NULL pointer deref/heap corruption

   In the BN_hex2bn function the number of hex digits is calculated using an
   int value `i`. Later `bn_expand` is called with a value of `i * 4`. For
   large values of `i` this can result in `bn_expand` not allocating any
   memory because `i * 4` is negative. This can leave the internal BIGNUM data
   field as NULL leading to a subsequent NULL ptr deref. For very large values
   of `i`, the calculation `i * 4` could be a positive value smaller than `i`.
   In this case memory is allocated to the internal BIGNUM data field, but it
   is insufficiently sized leading to heap corruption. A similar issue exists
   in BN_dec2bn. This could have security consequences if BN_hex2bn/BN_dec2bn
   is ever called by user applications with very large untrusted hex/dec data.
   This is anticipated to be a rare occurrence.

   All OpenSSL internal usage of these functions use data that is not expected
   to be untrusted, e.g. config file data or application command line
   arguments. If user developed applications generate config file data based
   on untrusted data then it is possible that this could also lead to security
   consequences. This is also anticipated to be rare.

   This issue was reported to OpenSSL by Guido Vranken.
   ([CVE-2016-0797])

   *Matt Caswell*

 * Fix memory issues in `BIO_*printf` functions

   The internal `fmtstr` function used in processing a "%s" format string in
   the `BIO_*printf` functions could overflow while calculating the length of a
   string and cause an OOB read when printing very long strings.

   Additionally the internal `doapr_outch` function can attempt to write to an
   OOB memory location (at an offset from the NULL pointer) in the event of a
   memory allocation failure. In 1.0.2 and below this could be caused where
   the size of a buffer to be allocated is greater than INT_MAX. E.g. this
   could be in processing a very long "%s" format string. Memory leaks can
   also occur.

   The first issue may mask the second issue dependent on compiler behaviour.
   These problems could enable attacks where large amounts of untrusted data
   is passed to the `BIO_*printf` functions. If applications use these functions
   in this way then they could be vulnerable. OpenSSL itself uses these
   functions when printing out human-readable dumps of ASN.1 data. Therefore
   applications that print this data could be vulnerable if the data is from
   untrusted sources. OpenSSL command line applications could also be
   vulnerable where they print out ASN.1 data, or if untrusted data is passed
   as command line arguments.

   Libssl is not considered directly vulnerable. Additionally certificates etc
   received via remote connections via libssl are also unlikely to be able to
   trigger these issues because of message size limits enforced within libssl.

   This issue was reported to OpenSSL Guido Vranken.
   ([CVE-2016-0799])

   *Matt Caswell*

 * Side channel attack on modular exponentiation

   A side-channel attack was found which makes use of cache-bank conflicts on
   the Intel Sandy-Bridge microarchitecture which could lead to the recovery
   of RSA keys.  The ability to exploit this issue is limited as it relies on
   an attacker who has control of code in a thread running on the same
   hyper-threaded core as the victim thread which is performing decryptions.

   This issue was reported to OpenSSL by Yuval Yarom, The University of
   Adelaide and NICTA, Daniel Genkin, Technion and Tel Aviv University, and
   Nadia Heninger, University of Pennsylvania with more information at
   <http://cachebleed.info>.
   ([CVE-2016-0702])

   *Andy Polyakov*

 * Change the `req` command to generate a 2048-bit RSA/DSA key by default,
   if no keysize is specified with default_bits. This fixes an
   omission in an earlier change that changed all RSA/DSA key generation
   commands to use 2048 bits by default.

   *Emilia Käsper*

### Changes between 1.0.2e and 1.0.2f [28 Jan 2016]

 * DH small subgroups

   Historically OpenSSL only ever generated DH parameters based on "safe"
   primes. More recently (in version 1.0.2) support was provided for
   generating X9.42 style parameter files such as those required for RFC 5114
   support. The primes used in such files may not be "safe". Where an
   application is using DH configured with parameters based on primes that are
   not "safe" then an attacker could use this fact to find a peer's private
   DH exponent. This attack requires that the attacker complete multiple
   handshakes in which the peer uses the same private DH exponent. For example
   this could be used to discover a TLS server's private DH exponent if it's
   reusing the private DH exponent or it's using a static DH ciphersuite.

   OpenSSL provides the option SSL_OP_SINGLE_DH_USE for ephemeral DH (DHE) in
   TLS. It is not on by default. If the option is not set then the server
   reuses the same private DH exponent for the life of the server process and
   would be vulnerable to this attack. It is believed that many popular
   applications do set this option and would therefore not be at risk.

   The fix for this issue adds an additional check where a "q" parameter is
   available (as is the case in X9.42 based parameters). This detects the
   only known attack, and is the only possible defense for static DH
   ciphersuites. This could have some performance impact.

   Additionally the SSL_OP_SINGLE_DH_USE option has been switched on by
   default and cannot be disabled. This could have some performance impact.

   This issue was reported to OpenSSL by Antonio Sanso (Adobe).
   ([CVE-2016-0701])

   *Matt Caswell*

 * SSLv2 doesn't block disabled ciphers

   A malicious client can negotiate SSLv2 ciphers that have been disabled on
   the server and complete SSLv2 handshakes even if all SSLv2 ciphers have
   been disabled, provided that the SSLv2 protocol was not also disabled via
   SSL_OP_NO_SSLv2.

   This issue was reported to OpenSSL on 26th December 2015 by Nimrod Aviram
   and Sebastian Schinzel.
   ([CVE-2015-3197])

   *Viktor Dukhovni*

### Changes between 1.0.2d and 1.0.2e [3 Dec 2015]

 * BN_mod_exp may produce incorrect results on x86_64

   There is a carry propagating bug in the x86_64 Montgomery squaring
   procedure. No EC algorithms are affected. Analysis suggests that attacks
   against RSA and DSA as a result of this defect would be very difficult to
   perform and are not believed likely. Attacks against DH are considered just
   feasible (although very difficult) because most of the work necessary to
   deduce information about a private key may be performed offline. The amount
   of resources required for such an attack would be very significant and
   likely only accessible to a limited number of attackers. An attacker would
   additionally need online access to an unpatched system using the target
   private key in a scenario with persistent DH parameters and a private
   key that is shared between multiple clients. For example this can occur by
   default in OpenSSL DHE based SSL/TLS ciphersuites.

   This issue was reported to OpenSSL by Hanno Böck.
   ([CVE-2015-3193])

   *Andy Polyakov*

 * Certificate verify crash with missing PSS parameter

   The signature verification routines will crash with a NULL pointer
   dereference if presented with an ASN.1 signature using the RSA PSS
   algorithm and absent mask generation function parameter. Since these
   routines are used to verify certificate signature algorithms this can be
   used to crash any certificate verification operation and exploited in a
   DoS attack. Any application which performs certificate verification is
   vulnerable including OpenSSL clients and servers which enable client
   authentication.

   This issue was reported to OpenSSL by Loïc Jonas Etienne (Qnective AG).
   ([CVE-2015-3194])

   *Stephen Henson*

 * X509_ATTRIBUTE memory leak

   When presented with a malformed X509_ATTRIBUTE structure OpenSSL will leak
   memory. This structure is used by the PKCS#7 and CMS routines so any
   application which reads PKCS#7 or CMS data from untrusted sources is
   affected. SSL/TLS is not affected.

   This issue was reported to OpenSSL by Adam Langley (Google/BoringSSL) using
   libFuzzer.
   ([CVE-2015-3195])

   *Stephen Henson*

 * Rewrite EVP_DecodeUpdate (base64 decoding) to fix several bugs.
   This changes the decoding behaviour for some invalid messages,
   though the change is mostly in the more lenient direction, and
   legacy behaviour is preserved as much as possible.

   *Emilia Käsper*

 * In DSA_generate_parameters_ex, if the provided seed is too short,
   return an error

   *Rich Salz and Ismo Puustinen <ismo.puustinen@intel.com>*

### Changes between 1.0.2c and 1.0.2d [9 Jul 2015]

 * Alternate chains certificate forgery

   During certificate verification, OpenSSL will attempt to find an
   alternative certificate chain if the first attempt to build such a chain
   fails. An error in the implementation of this logic can mean that an
   attacker could cause certain checks on untrusted certificates to be
   bypassed, such as the CA flag, enabling them to use a valid leaf
   certificate to act as a CA and "issue" an invalid certificate.

   This issue was reported to OpenSSL by Adam Langley/David Benjamin
   (Google/BoringSSL).

   *Matt Caswell*

### Changes between 1.0.2b and 1.0.2c [12 Jun 2015]

 * Fix HMAC ABI incompatibility. The previous version introduced an ABI
   incompatibility in the handling of HMAC. The previous ABI has now been
   restored.

   *Matt Caswell*

### Changes between 1.0.2a and 1.0.2b [11 Jun 2015]

 * Malformed ECParameters causes infinite loop

   When processing an ECParameters structure OpenSSL enters an infinite loop
   if the curve specified is over a specially malformed binary polynomial
   field.

   This can be used to perform denial of service against any
   system which processes public keys, certificate requests or
   certificates.  This includes TLS clients and TLS servers with
   client authentication enabled.

   This issue was reported to OpenSSL by Joseph Barr-Pixton.
   ([CVE-2015-1788])

   *Andy Polyakov*

 * Exploitable out-of-bounds read in X509_cmp_time

   X509_cmp_time does not properly check the length of the ASN1_TIME
   string and can read a few bytes out of bounds. In addition,
   X509_cmp_time accepts an arbitrary number of fractional seconds in the
   time string.

   An attacker can use this to craft malformed certificates and CRLs of
   various sizes and potentially cause a segmentation fault, resulting in
   a DoS on applications that verify certificates or CRLs. TLS clients
   that verify CRLs are affected. TLS clients and servers with client
   authentication enabled may be affected if they use custom verification
   callbacks.

   This issue was reported to OpenSSL by Robert Swiecki (Google), and
   independently by Hanno Böck.
   ([CVE-2015-1789])

   *Emilia Käsper*

 * PKCS7 crash with missing EnvelopedContent

   The PKCS#7 parsing code does not handle missing inner EncryptedContent
   correctly. An attacker can craft malformed ASN.1-encoded PKCS#7 blobs
   with missing content and trigger a NULL pointer dereference on parsing.

   Applications that decrypt PKCS#7 data or otherwise parse PKCS#7
   structures from untrusted sources are affected. OpenSSL clients and
   servers are not affected.

   This issue was reported to OpenSSL by Michal Zalewski (Google).
   ([CVE-2015-1790])

   *Emilia Käsper*

 * CMS verify infinite loop with unknown hash function

   When verifying a signedData message the CMS code can enter an infinite loop
   if presented with an unknown hash function OID. This can be used to perform
   denial of service against any system which verifies signedData messages using
   the CMS code.
   This issue was reported to OpenSSL by Johannes Bauer.
   ([CVE-2015-1792])

   *Stephen Henson*

 * Race condition handling NewSessionTicket

   If a NewSessionTicket is received by a multi-threaded client when attempting to
   reuse a previous ticket then a race condition can occur potentially leading to
   a double free of the ticket data.
   ([CVE-2015-1791])

   *Matt Caswell*

 * Only support 256-bit or stronger elliptic curves with the
   'ecdh_auto' setting (server) or by default (client). Of supported
   curves, prefer P-256 (both).

   *Emilia Kasper*

### Changes between 1.0.2 and 1.0.2a [19 Mar 2015]

 * ClientHello sigalgs DoS fix

   If a client connects to an OpenSSL 1.0.2 server and renegotiates with an
   invalid signature algorithms extension a NULL pointer dereference will
   occur. This can be exploited in a DoS attack against the server.

   This issue was was reported to OpenSSL by David Ramos of Stanford
   University.
   ([CVE-2015-0291])

   *Stephen Henson and Matt Caswell*

 * Multiblock corrupted pointer fix

   OpenSSL 1.0.2 introduced the "multiblock" performance improvement. This
   feature only applies on 64 bit x86 architecture platforms that support AES
   NI instructions. A defect in the implementation of "multiblock" can cause
   OpenSSL's internal write buffer to become incorrectly set to NULL when
   using non-blocking IO. Typically, when the user application is using a
   socket BIO for writing, this will only result in a failed connection.
   However if some other BIO is used then it is likely that a segmentation
   fault will be triggered, thus enabling a potential DoS attack.

   This issue was reported to OpenSSL by Daniel Danner and Rainer Mueller.
   ([CVE-2015-0290])

   *Matt Caswell*

 * Segmentation fault in DTLSv1_listen fix

   The DTLSv1_listen function is intended to be stateless and processes the
   initial ClientHello from many peers. It is common for user code to loop
   over the call to DTLSv1_listen until a valid ClientHello is received with
   an associated cookie. A defect in the implementation of DTLSv1_listen means
   that state is preserved in the SSL object from one invocation to the next
   that can lead to a segmentation fault. Errors processing the initial
   ClientHello can trigger this scenario. An example of such an error could be
   that a DTLS1.0 only client is attempting to connect to a DTLS1.2 only
   server.

   This issue was reported to OpenSSL by Per Allansson.
   ([CVE-2015-0207])

   *Matt Caswell*

 * Segmentation fault in ASN1_TYPE_cmp fix

   The function ASN1_TYPE_cmp will crash with an invalid read if an attempt is
   made to compare ASN.1 boolean types. Since ASN1_TYPE_cmp is used to check
   certificate signature algorithm consistency this can be used to crash any
   certificate verification operation and exploited in a DoS attack. Any
   application which performs certificate verification is vulnerable including
   OpenSSL clients and servers which enable client authentication.
   ([CVE-2015-0286])

   *Stephen Henson*

 * Segmentation fault for invalid PSS parameters fix

   The signature verification routines will crash with a NULL pointer
   dereference if presented with an ASN.1 signature using the RSA PSS
   algorithm and invalid parameters. Since these routines are used to verify
   certificate signature algorithms this can be used to crash any
   certificate verification operation and exploited in a DoS attack. Any
   application which performs certificate verification is vulnerable including
   OpenSSL clients and servers which enable client authentication.

   This issue was was reported to OpenSSL by Brian Carpenter.
   ([CVE-2015-0208])

   *Stephen Henson*

 * ASN.1 structure reuse memory corruption fix

   Reusing a structure in ASN.1 parsing may allow an attacker to cause
   memory corruption via an invalid write. Such reuse is and has been
   strongly discouraged and is believed to be rare.

   Applications that parse structures containing CHOICE or ANY DEFINED BY
   components may be affected. Certificate parsing (d2i_X509 and related
   functions) are however not affected. OpenSSL clients and servers are
   not affected.
   ([CVE-2015-0287])

   *Stephen Henson*

 * PKCS7 NULL pointer dereferences fix

   The PKCS#7 parsing code does not handle missing outer ContentInfo
   correctly. An attacker can craft malformed ASN.1-encoded PKCS#7 blobs with
   missing content and trigger a NULL pointer dereference on parsing.

   Applications that verify PKCS#7 signatures, decrypt PKCS#7 data or
   otherwise parse PKCS#7 structures from untrusted sources are
   affected. OpenSSL clients and servers are not affected.

   This issue was reported to OpenSSL by Michal Zalewski (Google).
   ([CVE-2015-0289])

   *Emilia Käsper*

 * DoS via reachable assert in SSLv2 servers fix

   A malicious client can trigger an OPENSSL_assert (i.e., an abort) in
   servers that both support SSLv2 and enable export cipher suites by sending
   a specially crafted SSLv2 CLIENT-MASTER-KEY message.

   This issue was discovered by Sean Burford (Google) and Emilia Käsper
   (OpenSSL development team).
   ([CVE-2015-0293])

   *Emilia Käsper*

 * Empty CKE with client auth and DHE fix

   If client auth is used then a server can seg fault in the event of a DHE
   ciphersuite being selected and a zero length ClientKeyExchange message
   being sent by the client. This could be exploited in a DoS attack.
   ([CVE-2015-1787])

   *Matt Caswell*

 * Handshake with unseeded PRNG fix

   Under certain conditions an OpenSSL 1.0.2 client can complete a handshake
   with an unseeded PRNG. The conditions are:
   - The client is on a platform where the PRNG has not been seeded
   automatically, and the user has not seeded manually
   - A protocol specific client method version has been used (i.e. not
   SSL_client_methodv23)
   - A ciphersuite is used that does not require additional random data from
   the PRNG beyond the initial ClientHello client random (e.g. PSK-RC4-SHA).

   If the handshake succeeds then the client random that has been used will
   have been generated from a PRNG with insufficient entropy and therefore the
   output may be predictable.

   For example using the following command with an unseeded openssl will
   succeed on an unpatched platform:

   openssl s_client -psk 1a2b3c4d -tls1_2 -cipher PSK-RC4-SHA
   ([CVE-2015-0285])

   *Matt Caswell*

 * Use After Free following d2i_ECPrivatekey error fix

   A malformed EC private key file consumed via the d2i_ECPrivateKey function
   could cause a use after free condition. This, in turn, could cause a double
   free in several private key parsing functions (such as d2i_PrivateKey
   or EVP_PKCS82PKEY) and could lead to a DoS attack or memory corruption
   for applications that receive EC private keys from untrusted
   sources. This scenario is considered rare.

   This issue was discovered by the BoringSSL project and fixed in their
   commit 517073cd4b.
   ([CVE-2015-0209])

   *Matt Caswell*

 * X509_to_X509_REQ NULL pointer deref fix

   The function X509_to_X509_REQ will crash with a NULL pointer dereference if
   the certificate key is invalid. This function is rarely used in practice.

   This issue was discovered by Brian Carpenter.
   ([CVE-2015-0288])

   *Stephen Henson*

 * Removed the export ciphers from the DEFAULT ciphers

   *Kurt Roeckx*

### Changes between 1.0.1l and 1.0.2 [22 Jan 2015]

 * Facilitate "universal" ARM builds targeting range of ARM ISAs, e.g.
   ARMv5 through ARMv8, as opposite to "locking" it to single one.
   So far those who have to target multiple platforms would compromise
   and argue that binary targeting say ARMv5 would still execute on
   ARMv8. "Universal" build resolves this compromise by providing
   near-optimal performance even on newer platforms.

   *Andy Polyakov*

 * Accelerated NIST P-256 elliptic curve implementation for x86_64
   (other platforms pending).

   *Shay Gueron & Vlad Krasnov (Intel Corp), Andy Polyakov*

 * Add support for the SignedCertificateTimestampList certificate and
   OCSP response extensions from RFC6962.

   *Rob Stradling*

 * Fix ec_GFp_simple_points_make_affine (thus, EC_POINTs_mul etc.)
   for corner cases. (Certain input points at infinity could lead to
   bogus results, with non-infinity inputs mapped to infinity too.)

   *Bodo Moeller*

 * Initial support for PowerISA 2.0.7, first implemented in POWER8.
   This covers AES, SHA256/512 and GHASH. "Initial" means that most
   common cases are optimized and there still is room for further
   improvements. Vector Permutation AES for Altivec is also added.

   *Andy Polyakov*

 * Add support for little-endian ppc64 Linux target.

   *Marcelo Cerri (IBM)*

 * Initial support for AMRv8 ISA crypto extensions. This covers AES,
   SHA1, SHA256 and GHASH. "Initial" means that most common cases
   are optimized and there still is room for further improvements.
   Both 32- and 64-bit modes are supported.

   *Andy Polyakov, Ard Biesheuvel (Linaro)*

 * Improved ARMv7 NEON support.

   *Andy Polyakov*

 * Support for SPARC Architecture 2011 crypto extensions, first
   implemented in SPARC T4. This covers AES, DES, Camellia, SHA1,
   SHA256/512, MD5, GHASH and modular exponentiation.

   *Andy Polyakov, David Miller*

 * Accelerated modular exponentiation for Intel processors, a.k.a.
   RSAZ.

   *Shay Gueron & Vlad Krasnov (Intel Corp)*

 * Support for new and upcoming Intel processors, including AVX2,
   BMI and SHA ISA extensions. This includes additional "stitched"
   implementations, AESNI-SHA256 and GCM, and multi-buffer support
   for TLS encrypt.

   This work was sponsored by Intel Corp.

   *Andy Polyakov*

 * Support for DTLS 1.2. This adds two sets of DTLS methods: DTLS_*_method()
   supports both DTLS 1.2 and 1.0 and should use whatever version the peer
   supports and DTLSv1_2_*_method() which supports DTLS 1.2 only.

   *Steve Henson*

 * Use algorithm specific chains in SSL_CTX_use_certificate_chain_file():
   this fixes a limitation in previous versions of OpenSSL.

   *Steve Henson*

 * Extended RSA OAEP support via EVP_PKEY API. Options to specify digest,
   MGF1 digest and OAEP label.

   *Steve Henson*

 * Add EVP support for key wrapping algorithms, to avoid problems with
   existing code the flag EVP_CIPHER_CTX_WRAP_ALLOW has to be set in
   the EVP_CIPHER_CTX or an error is returned. Add AES and DES3 wrap
   algorithms and include tests cases.

   *Steve Henson*

 * Add functions to allocate and set the fields of an ECDSA_METHOD
   structure.

   *Douglas E. Engert, Steve Henson*

 * New functions OPENSSL_gmtime_diff and ASN1_TIME_diff to find the
   difference in days and seconds between two tm or ASN1_TIME structures.

   *Steve Henson*

 * Add -rev test option to s_server to just reverse order of characters
   received by client and send back to server. Also prints an abbreviated
   summary of the connection parameters.

   *Steve Henson*

 * New option -brief for s_client and s_server to print out a brief summary
   of connection parameters.

   *Steve Henson*

 * Add callbacks for arbitrary TLS extensions.

   *Trevor Perrin <trevp@trevp.net> and Ben Laurie*

 * New option -crl_download in several openssl utilities to download CRLs
   from CRLDP extension in certificates.

   *Steve Henson*

 * New options -CRL and -CRLform for s_client and s_server for CRLs.

   *Steve Henson*

 * New function X509_CRL_diff to generate a delta CRL from the difference
   of two full CRLs. Add support to "crl" utility.

   *Steve Henson*

 * New functions to set lookup_crls function and to retrieve
   X509_STORE from X509_STORE_CTX.

   *Steve Henson*

 * Print out deprecated issuer and subject unique ID fields in
   certificates.

   *Steve Henson*

 * Extend OCSP I/O functions so they can be used for simple general purpose
   HTTP as well as OCSP. New wrapper function which can be used to download
   CRLs using the OCSP API.

   *Steve Henson*

 * Delegate command line handling in s_client/s_server to SSL_CONF APIs.

   *Steve Henson*

 * `SSL_CONF*` functions. These provide a common framework for application
   configuration using configuration files or command lines.

   *Steve Henson*

 * SSL/TLS tracing code. This parses out SSL/TLS records using the
   message callback and prints the results. Needs compile time option
   "enable-ssl-trace". New options to s_client and s_server to enable
   tracing.

   *Steve Henson*

 * New ctrl and macro to retrieve supported points extensions.
   Print out extension in s_server and s_client.

   *Steve Henson*

 * New functions to retrieve certificate signature and signature
   OID NID.

   *Steve Henson*

 * Add functions to retrieve and manipulate the raw cipherlist sent by a
   client to OpenSSL.

   *Steve Henson*

 * New Suite B modes for TLS code. These use and enforce the requirements
   of RFC6460: restrict ciphersuites, only permit Suite B algorithms and
   only use Suite B curves. The Suite B modes can be set by using the
   strings "SUITEB128", "SUITEB192" or "SUITEB128ONLY" for the cipherstring.

   *Steve Henson*

 * New chain verification flags for Suite B levels of security. Check
   algorithms are acceptable when flags are set in X509_verify_cert.

   *Steve Henson*

 * Make tls1_check_chain return a set of flags indicating checks passed
   by a certificate chain. Add additional tests to handle client
   certificates: checks for matching certificate type and issuer name
   comparison.

   *Steve Henson*

 * If an attempt is made to use a signature algorithm not in the peer
   preference list abort the handshake. If client has no suitable
   signature algorithms in response to a certificate request do not
   use the certificate.

   *Steve Henson*

 * If server EC tmp key is not in client preference list abort handshake.

   *Steve Henson*

 * Add support for certificate stores in CERT structure. This makes it
   possible to have different stores per SSL structure or one store in
   the parent SSL_CTX. Include distinct stores for certificate chain
   verification and chain building. New ctrl SSL_CTRL_BUILD_CERT_CHAIN
   to build and store a certificate chain in CERT structure: returning
   an error if the chain cannot be built: this will allow applications
   to test if a chain is correctly configured.

   Note: if the CERT based stores are not set then the parent SSL_CTX
   store is used to retain compatibility with existing behaviour.

   *Steve Henson*

 * New function ssl_set_client_disabled to set a ciphersuite disabled
   mask based on the current session, check mask when sending client
   hello and checking the requested ciphersuite.

   *Steve Henson*

 * New ctrls to retrieve and set certificate types in a certificate
   request message. Print out received values in s_client. If certificate
   types is not set with custom values set sensible values based on
   supported signature algorithms.

   *Steve Henson*

 * Support for distinct client and server supported signature algorithms.

   *Steve Henson*

 * Add certificate callback. If set this is called whenever a certificate
   is required by client or server. An application can decide which
   certificate chain to present based on arbitrary criteria: for example
   supported signature algorithms. Add very simple example to s_server.
   This fixes many of the problems and restrictions of the existing client
   certificate callback: for example you can now clear an existing
   certificate and specify the whole chain.

   *Steve Henson*

 * Add new "valid_flags" field to CERT_PKEY structure which determines what
   the certificate can be used for (if anything). Set valid_flags field
   in new tls1_check_chain function. Simplify ssl_set_cert_masks which used
   to have similar checks in it.

   Add new "cert_flags" field to CERT structure and include a "strict mode".
   This enforces some TLS certificate requirements (such as only permitting
   certificate signature algorithms contained in the supported algorithms
   extension) which some implementations ignore: this option should be used
   with caution as it could cause interoperability issues.

   *Steve Henson*

 * Update and tidy signature algorithm extension processing. Work out
   shared signature algorithms based on preferences and peer algorithms
   and print them out in s_client and s_server. Abort handshake if no
   shared signature algorithms.

   *Steve Henson*

 * Add new functions to allow customised supported signature algorithms
   for SSL and SSL_CTX structures. Add options to s_client and s_server
   to support them.

   *Steve Henson*

 * New function SSL_certs_clear() to delete all references to certificates
   from an SSL structure. Before this once a certificate had been added
   it couldn't be removed.

   *Steve Henson*

 * Integrate hostname, email address and IP address checking with certificate
   verification. New verify options supporting checking in openssl utility.

   *Steve Henson*

 * Fixes and wildcard matching support to hostname and email checking
   functions. Add manual page.

   *Florian Weimer (Red Hat Product Security Team)*

 * New functions to check a hostname email or IP address against a
   certificate. Add options x509 utility to print results of checks against
   a certificate.

   *Steve Henson*

 * Fix OCSP checking.

   *Rob Stradling <rob.stradling@comodo.com> and Ben Laurie*

 * Initial experimental support for explicitly trusted non-root CAs.
   OpenSSL still tries to build a complete chain to a root but if an
   intermediate CA has a trust setting included that is used. The first
   setting is used: whether to trust (e.g., -addtrust option to the x509
   utility) or reject.

   *Steve Henson*

 * Add -trusted_first option which attempts to find certificates in the
   trusted store even if an untrusted chain is also supplied.

   *Steve Henson*

 * MIPS assembly pack updates: support for MIPS32r2 and SmartMIPS ASE,
   platform support for Linux and Android.

   *Andy Polyakov*

 * Support for linux-x32, ILP32 environment in x86_64 framework.

   *Andy Polyakov*

 * Experimental multi-implementation support for FIPS capable OpenSSL.
   When in FIPS mode the approved implementations are used as normal,
   when not in FIPS mode the internal unapproved versions are used instead.
   This means that the FIPS capable OpenSSL isn't forced to use the
   (often lower performance) FIPS implementations outside FIPS mode.

   *Steve Henson*

 * Transparently support X9.42 DH parameters when calling
   PEM_read_bio_DHparameters. This means existing applications can handle
   the new parameter format automatically.

   *Steve Henson*

 * Initial experimental support for X9.42 DH parameter format: mainly
   to support use of 'q' parameter for RFC5114 parameters.

   *Steve Henson*

 * Add DH parameters from RFC5114 including test data to dhtest.

   *Steve Henson*

 * Support for automatic EC temporary key parameter selection. If enabled
   the most preferred EC parameters are automatically used instead of
   hardcoded fixed parameters. Now a server just has to call:
   SSL_CTX_set_ecdh_auto(ctx, 1) and the server will automatically
   support ECDH and use the most appropriate parameters.

   *Steve Henson*

 * Enhance and tidy EC curve and point format TLS extension code. Use
   static structures instead of allocation if default values are used.
   New ctrls to set curves we wish to support and to retrieve shared curves.
   Print out shared curves in s_server. New options to s_server and s_client
   to set list of supported curves.

   *Steve Henson*

 * New ctrls to retrieve supported signature algorithms and
   supported curve values as an array of NIDs. Extend openssl utility
   to print out received values.

   *Steve Henson*

 * Add new APIs EC_curve_nist2nid and EC_curve_nid2nist which convert
   between NIDs and the more common NIST names such as "P-256". Enhance
   ecparam utility and ECC method to recognise the NIST names for curves.

   *Steve Henson*

 * Enhance SSL/TLS certificate chain handling to support different
   chains for each certificate instead of one chain in the parent SSL_CTX.

   *Steve Henson*

 * Support for fixed DH ciphersuite client authentication: where both
   server and client use DH certificates with common parameters.

   *Steve Henson*

 * Support for fixed DH ciphersuites: those requiring DH server
   certificates.

   *Steve Henson*

 * New function i2d_re_X509_tbs for re-encoding the TBS portion of
   the certificate.
   Note: Related 1.0.2-beta specific macros X509_get_cert_info,
   X509_CINF_set_modified, X509_CINF_get_issuer, X509_CINF_get_extensions and
   X509_CINF_get_signature were reverted post internal team review.

OpenSSL 1.0.1
-------------

### Changes between 1.0.1t and 1.0.1u [22 Sep 2016]

 * OCSP Status Request extension unbounded memory growth

   A malicious client can send an excessively large OCSP Status Request
   extension. If that client continually requests renegotiation, sending a
   large OCSP Status Request extension each time, then there will be unbounded
   memory growth on the server. This will eventually lead to a Denial Of
   Service attack through memory exhaustion. Servers with a default
   configuration are vulnerable even if they do not support OCSP. Builds using
   the "no-ocsp" build time option are not affected.

   This issue was reported to OpenSSL by Shi Lei (Gear Team, Qihoo 360 Inc.)
   ([CVE-2016-6304])

   *Matt Caswell*

 * In order to mitigate the SWEET32 attack, the DES ciphers were moved from
   HIGH to MEDIUM.

   This issue was reported to OpenSSL Karthikeyan Bhargavan and Gaetan
   Leurent (INRIA)
   ([CVE-2016-2183])

   *Rich Salz*

 * OOB write in MDC2_Update()

   An overflow can occur in MDC2_Update() either if called directly or
   through the EVP_DigestUpdate() function using MDC2. If an attacker
   is able to supply very large amounts of input data after a previous
   call to EVP_EncryptUpdate() with a partial block then a length check
   can overflow resulting in a heap corruption.

   The amount of data needed is comparable to SIZE_MAX which is impractical
   on most platforms.

   This issue was reported to OpenSSL by Shi Lei (Gear Team, Qihoo 360 Inc.)
   ([CVE-2016-6303])

   *Stephen Henson*

 * Malformed SHA512 ticket DoS

   If a server uses SHA512 for TLS session ticket HMAC it is vulnerable to a
   DoS attack where a malformed ticket will result in an OOB read which will
   ultimately crash.

   The use of SHA512 in TLS session tickets is comparatively rare as it requires
   a custom server callback and ticket lookup mechanism.

   This issue was reported to OpenSSL by Shi Lei (Gear Team, Qihoo 360 Inc.)
   ([CVE-2016-6302])

   *Stephen Henson*

 * OOB write in BN_bn2dec()

   The function BN_bn2dec() does not check the return value of BN_div_word().
   This can cause an OOB write if an application uses this function with an
   overly large BIGNUM. This could be a problem if an overly large certificate
   or CRL is printed out from an untrusted source. TLS is not affected because
   record limits will reject an oversized certificate before it is parsed.

   This issue was reported to OpenSSL by Shi Lei (Gear Team, Qihoo 360 Inc.)
   ([CVE-2016-2182])

   *Stephen Henson*

 * OOB read in TS_OBJ_print_bio()

   The function TS_OBJ_print_bio() misuses OBJ_obj2txt(): the return value is
   the total length the OID text representation would use and not the amount
   of data written. This will result in OOB reads when large OIDs are
   presented.

   This issue was reported to OpenSSL by Shi Lei (Gear Team, Qihoo 360 Inc.)
   ([CVE-2016-2180])

   *Stephen Henson*

 * Pointer arithmetic undefined behaviour

   Avoid some undefined pointer arithmetic

   A common idiom in the codebase is to check limits in the following manner:
   "p + len > limit"

   Where "p" points to some malloc'd data of SIZE bytes and
   limit == p + SIZE

   "len" here could be from some externally supplied data (e.g. from a TLS
   message).

   The rules of C pointer arithmetic are such that "p + len" is only well
   defined where len <= SIZE. Therefore the above idiom is actually
   undefined behaviour.

   For example this could cause problems if some malloc implementation
   provides an address for "p" such that "p + len" actually overflows for
   values of len that are too big and therefore p + len < limit.

   This issue was reported to OpenSSL by Guido Vranken
   ([CVE-2016-2177])

   *Matt Caswell*

 * Constant time flag not preserved in DSA signing

   Operations in the DSA signing algorithm should run in constant time in
   order to avoid side channel attacks. A flaw in the OpenSSL DSA
   implementation means that a non-constant time codepath is followed for
   certain operations. This has been demonstrated through a cache-timing
   attack to be sufficient for an attacker to recover the private DSA key.

   This issue was reported by César Pereida (Aalto University), Billy Brumley
   (Tampere University of Technology), and Yuval Yarom (The University of
   Adelaide and NICTA).
   ([CVE-2016-2178])

   *César Pereida*

 * DTLS buffered message DoS

   In a DTLS connection where handshake messages are delivered out-of-order
   those messages that OpenSSL is not yet ready to process will be buffered
   for later use. Under certain circumstances, a flaw in the logic means that
   those messages do not get removed from the buffer even though the handshake
   has been completed. An attacker could force up to approx. 15 messages to
   remain in the buffer when they are no longer required. These messages will
   be cleared when the DTLS connection is closed. The default maximum size for
   a message is 100k. Therefore the attacker could force an additional 1500k
   to be consumed per connection. By opening many simulataneous connections an
   attacker could cause a DoS attack through memory exhaustion.

   This issue was reported to OpenSSL by Quan Luo.
   ([CVE-2016-2179])

   *Matt Caswell*

 * DTLS replay protection DoS

   A flaw in the DTLS replay attack protection mechanism means that records
   that arrive for future epochs update the replay protection "window" before
   the MAC for the record has been validated. This could be exploited by an
   attacker by sending a record for the next epoch (which does not have to
   decrypt or have a valid MAC), with a very large sequence number. This means
   that all subsequent legitimate packets are dropped causing a denial of
   service for a specific DTLS connection.

   This issue was reported to OpenSSL by the OCAP audit team.
   ([CVE-2016-2181])

   *Matt Caswell*

 * Certificate message OOB reads

   In OpenSSL 1.0.2 and earlier some missing message length checks can result
   in OOB reads of up to 2 bytes beyond an allocated buffer. There is a
   theoretical DoS risk but this has not been observed in practice on common
   platforms.

   The messages affected are client certificate, client certificate request
   and server certificate. As a result the attack can only be performed
   against a client or a server which enables client authentication.

   This issue was reported to OpenSSL by Shi Lei (Gear Team, Qihoo 360 Inc.)
   ([CVE-2016-6306])

   *Stephen Henson*

### Changes between 1.0.1s and 1.0.1t [3 May 2016]

 * Prevent padding oracle in AES-NI CBC MAC check

   A MITM attacker can use a padding oracle attack to decrypt traffic
   when the connection uses an AES CBC cipher and the server support
   AES-NI.

   This issue was introduced as part of the fix for Lucky 13 padding
   attack ([CVE-2013-0169]). The padding check was rewritten to be in
   constant time by making sure that always the same bytes are read and
   compared against either the MAC or padding bytes. But it no longer
   checked that there was enough data to have both the MAC and padding
   bytes.

   This issue was reported by Juraj Somorovsky using TLS-Attacker.
   ([CVE-2016-2107])

   *Kurt Roeckx*

 * Fix EVP_EncodeUpdate overflow

   An overflow can occur in the EVP_EncodeUpdate() function which is used for
   Base64 encoding of binary data. If an attacker is able to supply very large
   amounts of input data then a length check can overflow resulting in a heap
   corruption.

   Internally to OpenSSL the EVP_EncodeUpdate() function is primarly used by
   the `PEM_write_bio*` family of functions. These are mainly used within the
   OpenSSL command line applications, so any application which processes data
   from an untrusted source and outputs it as a PEM file should be considered
   vulnerable to this issue. User applications that call these APIs directly
   with large amounts of untrusted data may also be vulnerable.

   This issue was reported by Guido Vranken.
   ([CVE-2016-2105])

   *Matt Caswell*

 * Fix EVP_EncryptUpdate overflow

   An overflow can occur in the EVP_EncryptUpdate() function. If an attacker
   is able to supply very large amounts of input data after a previous call to
   EVP_EncryptUpdate() with a partial block then a length check can overflow
   resulting in a heap corruption. Following an analysis of all OpenSSL
   internal usage of the EVP_EncryptUpdate() function all usage is one of two
   forms. The first form is where the EVP_EncryptUpdate() call is known to be
   the first called function after an EVP_EncryptInit(), and therefore that
   specific call must be safe. The second form is where the length passed to
   EVP_EncryptUpdate() can be seen from the code to be some small value and
   therefore there is no possibility of an overflow. Since all instances are
   one of these two forms, it is believed that there can be no overflows in
   internal code due to this problem. It should be noted that
   EVP_DecryptUpdate() can call EVP_EncryptUpdate() in certain code paths.
   Also EVP_CipherUpdate() is a synonym for EVP_EncryptUpdate(). All instances
   of these calls have also been analysed too and it is believed there are no
   instances in internal usage where an overflow could occur.

   This issue was reported by Guido Vranken.
   ([CVE-2016-2106])

   *Matt Caswell*

 * Prevent ASN.1 BIO excessive memory allocation

   When ASN.1 data is read from a BIO using functions such as d2i_CMS_bio()
   a short invalid encoding can casuse allocation of lar