FROM debian:9 as build
RUN apt-get update \
	&& DEBIAN_FRONTEND=noninteractive apt-get install --no-install-recommends --no-install-suggests -y \
	cmake make gcc libc-dev \
	&& apt-get clean \
	&& rm -vrf /var/lib/apt/lists/*
ADD . /nosu
RUN rm -vrf /nosu/build \
	&& mkdir /nosu/build \
	&& cd /nosu/build \
	&& cmake .. \
	&& make \
	&& mv nosu /usr/local/bin/nosu \
	&& cd .. \
	&& rm -vrf build

FROM debian:9
COPY --from=build /usr/local/bin/nosu /usr/local/bin/nosu
