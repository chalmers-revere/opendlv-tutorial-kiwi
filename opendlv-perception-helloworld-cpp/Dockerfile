# Copyright (C) 2018  Christian Berger
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

FROM archlinux as builder

RUN pacman -Syu --noconfirm
RUN pacman -S --noconfirm opencv qt5-base hdf5 vtk glew
RUN pacman -S --noconfirm base-devel cmake

ADD . /opt/sources
WORKDIR /opt/sources
RUN mkdir build && \
    cd build && \
    cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/tmp/dest .. && \
    make && make install


FROM archlinux

RUN pacman -Syu --noconfirm
RUN pacman -S --noconfirm opencv qt5-base hdf5 vtk glew

WORKDIR /usr/bin
COPY --from=builder /tmp/dest /usr
ENTRYPOINT ["/usr/bin/opendlv-perception-helloworld"]
