# ex:ts=8
# Ports collection makefile for:	white_dune
# Date created:			Apr 12, 2002
# Whom:				ijliao
#
# $FreeBSD: ports/graphics/white_dune/Makefile,v 1.1 2002/04/11 19:25:29 ijliao Exp $
#

PORTNAME=	white_dune
PORTVERSION=	0.20b186
CATEGORIES=	graphics
MASTER_SITES=	http://wdune.ourproject.org/
DISTNAME=	${PORTNAME}-${PORTVERSION:S/b/beta/}

MAINTAINER=	ports@FreeBSD.org

LIB_DEPENDS=	png:${PORTSDIR}/graphics/png \
		jpeg:${PORTSDIR}/graphics/jpeg \
                lesstif:${PORTSDIR}/x11-toolkits/lesstif \
                mesa3:${PORTSDIR}/graphics/Mesa3

# libsball not in ports yet...

USE_BISON=	yes
USE_MESA=	yes
USE_MOTIF=	yes
GNU_CONFIGURE=	yes
ALL_TARGET=	# empty

MAN1=	man/dune.1

post-patch:
	echo nothing to do yet

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/dune ${PREFIX}/bin
	${INSTALL_PROGRAM} ${WRKSRC}/dune4kids ${PREFIX}/bin
	${INSTALL_MAN} ${WRKSRC}/${MAN1} ${MAN1PREFIX}/man/man1
.if !defined(NOPORTDOCS)
	@${MKDIR} ${DOCSDIR}
	${CP} -R ${WRKSRC}/docs/* ${DOCSDIR}
.endif

.include <bsd.port.mk>
